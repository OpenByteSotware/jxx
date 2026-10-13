#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"

#include <algorithm>
#include <limits>
#include <string>

#include "lang/jxx.lang.String.h"

#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.CancellableTask.h"

namespace jxx::util::concurrent {

ThreadPoolExecutor::ThreadPoolExecutor(
    ::jxx::lang::jint core,
    ::jxx::lang::jint maximum,
    ::jxx::lang::jlong keepAlive,
    const ::jxx::Ptr<TimeUnit>& unit)
    : ThreadPoolExecutor(core, maximum, keepAlive, unit,
          Executors::defaultThreadFactory()) {}

ThreadPoolExecutor::ThreadPoolExecutor(
    ::jxx::lang::jint core,
    ::jxx::lang::jint maximum,
    ::jxx::lang::jlong keepAlive,
    const ::jxx::Ptr<TimeUnit>& unit,
    const ::jxx::Ptr<ThreadFactory>& threadFactory)
    : corePoolSize_(core)
    , maximumPoolSize_(maximum)
    , keepAliveTime_(keepAlive)
    , keepAliveUnit_(unit)
    , threadFactory_(threadFactory) {
    if (core < 0 || maximum <= 0 || maximum < core || keepAlive < 0) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    if (unit == nullptr || threadFactory == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    shutdown();
    joinWorkers_();
}

void ThreadPoolExecutor::joinWorkers_() {
    for (const auto& worker : workers_) {
        if (worker != nullptr && worker->isAlive()) worker->join();
    }
}

::jxx::lang::jbool ThreadPoolExecutor::cachedMode_() const noexcept {
    return corePoolSize_ == 0 &&
        maximumPoolSize_ == std::numeric_limits<::jxx::lang::jint>::max();
}

::jxx::lang::jbool ThreadPoolExecutor::startWorkerLocked_() {
    const auto runnable = ::jxx::CAST<::jxx::lang::Runnable>(
        ::jxx::NEW<WorkerRunnable>(this));
    ::jxx::Ptr<::jxx::lang::Thread> worker;
    try {
        worker = threadFactory_->newThread(runnable);
        if (worker == nullptr) return false;
        worker->start();
    } catch (...) {
        return false;
    }

    workers_.push_back(worker);
    ++liveWorkerCount_;
    largestPoolSize_ = std::max(largestPoolSize_, liveWorkerCount_);
    return true;
}

void ThreadPoolExecutor::execute(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command) {
    if (command == nullptr) throw ::jxx::lang::NullPointerException();

    ::jxx::Ptr<RejectedExecutionHandler> rejectionHandler;
    bool rejected = false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (shutdown_) {
            rejected = true;
            rejectionHandler = handler_;
        }
        else {
            queue_.push_back(command);
            ++submittedTaskCount_;
            if (liveWorkerCount_ < corePoolSize_ ||
                (liveWorkerCount_ == 0 && !queue_.empty()) ||
                (cachedMode_() && liveWorkerCount_ < maximumPoolSize_ &&
                 activeCount_ + static_cast<::jxx::lang::jint>(queue_.size()) > liveWorkerCount_)) {
                if (!startWorkerLocked_() && liveWorkerCount_ == 0) {
                    queue_.pop_back();
                    --submittedTaskCount_;
                    rejected = true;
                    rejectionHandler = handler_;
                }
            }
        }
    }

    if (rejected) {
        if (rejectionHandler != nullptr) {
            rejectionHandler->rejectedExecution(
                command, ::jxx::CAST<ThreadPoolExecutor>(thisPtr()));
            return;
        }
        throw RejectedExecutionException(
            "task rejected from ThreadPoolExecutor");
    }

    // The shutdown decision was made atomically while mutex_ was held above.
    // Do not call the virtual isShutdown() here: a scheduled executor may have
    // begun orderly shutdown while still being required to dispatch retained
    // delayed tasks through this base executor.
    workAvailable_.notify_one();
}

void ThreadPoolExecutor::workerLoop_() {
    for (;;) {
        ::jxx::Ptr<::jxx::lang::Runnable> task;
        {
            std::unique_lock<std::mutex> lock(mutex_);
            const auto shouldStop = [&] {
                return stopNow_ || !queue_.empty() || shutdown_;
            };
            if (liveWorkerCount_ > corePoolSize_) {
                if (!workAvailable_.wait_for(lock, keepAliveUnit_->toChrono(keepAliveTime_), shouldStop)) {
                    --liveWorkerCount_;
                    terminated_.notify_all();
                    return;
                }
            }
            else {
                workAvailable_.wait(lock, shouldStop);
            }
            if (stopNow_ || (shutdown_ && queue_.empty())) {
                --liveWorkerCount_;
                terminated_.notify_all();
                return;
            }
            if (queue_.empty()) continue;
            task = queue_.front();
            queue_.pop_front();
            ++activeCount_;
        }
        // A worker reused for another task must not leak an interrupt that
        // was delivered to the preceding task. Immediate shutdown is the
        // exception: its interrupt must remain visible to the worker task.
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (!stopNow_) (void)::jxx::lang::Thread::interrupted();
        }
        try { task->run(); } catch (...) {}
        {
            std::lock_guard<std::mutex> lock(mutex_);
            --activeCount_;
            ++completedTaskCount_;
            if (shutdown_ && queue_.empty() && activeCount_ == 0) {
                workAvailable_.notify_all();
                terminated_.notify_all();
            }
        }
    }
}

void ThreadPoolExecutor::shutdown() {
    {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
    }
    workAvailable_.notify_all();
}

::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>>
ThreadPoolExecutor::shutdownNow() {
    auto result = ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::Runnable>>();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        shutdown_ = true;
        stopNow_ = true;
        while (!queue_.empty()) {
            result->add(queue_.front());
            queue_.pop_front();
        }
    }
    workAvailable_.notify_all();
    for (const auto& worker : workers_) {
        if (worker != nullptr && worker->isAlive()) worker->interrupt();
    }
    return ::jxx::CAST<::jxx::util::List<::jxx::lang::Runnable>>(result);
}

::jxx::lang::jbool ThreadPoolExecutor::isShutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_;
}

::jxx::lang::jbool ThreadPoolExecutor::isTerminated() {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_ && queue_.empty() && activeCount_ == 0 && liveWorkerCount_ == 0;
}

::jxx::lang::jbool ThreadPoolExecutor::awaitTermination(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<TimeUnit>& unit) {
    if (unit == nullptr) throw ::jxx::lang::NullPointerException();

    const auto current = ::jxx::lang::Thread::currentThread();
    if (::jxx::lang::Thread::interrupted()) {
        throw ::jxx::lang::InterruptedException();
    }

    std::unique_lock<std::mutex> lock(mutex_);
    const auto terminated = [this] {
        return shutdown_ && queue_.empty() &&
            activeCount_ == 0 && liveWorkerCount_ == 0;
    };
    if (terminated()) return true;
    if (timeout <= 0) return false;

    const auto duration = unit->toChrono(timeout);
    const auto now = std::chrono::steady_clock::now();
    const auto remaining = std::chrono::steady_clock::time_point::max() - now;
    const auto deadline = duration >= remaining
        ? std::chrono::steady_clock::time_point::max()
        : now + duration;

    current->setParkWakeup_([this] { terminated_.notify_all(); });
    try {
        const auto completed = terminated_.wait_until(
            lock,
            deadline,
            [&] { return terminated() || current->isInterrupted(); });
        current->clearParkWakeup_();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        return completed && terminated();
    } catch (...) {
        current->clearParkWakeup_();
        throw;
    }
}

::jxx::lang::jint ThreadPoolExecutor::getCorePoolSize() const noexcept { return corePoolSize_; }
::jxx::lang::jint ThreadPoolExecutor::getMaximumPoolSize() const noexcept { return maximumPoolSize_; }
::jxx::lang::jlong ThreadPoolExecutor::getKeepAliveTime(const ::jxx::Ptr<TimeUnit>& unit) const {
    if (unit == nullptr) throw ::jxx::lang::NullPointerException();
    return unit->convert(keepAliveTime_, keepAliveUnit_);
}
::jxx::lang::jint ThreadPoolExecutor::getPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return liveWorkerCount_;
}
::jxx::lang::jint ThreadPoolExecutor::getActiveCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return activeCount_;
}
::jxx::lang::jint ThreadPoolExecutor::getLargestPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return largestPoolSize_;
}
::jxx::lang::jlong ThreadPoolExecutor::getCompletedTaskCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return completedTaskCount_;
}
::jxx::lang::jlong ThreadPoolExecutor::getTaskCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return submittedTaskCount_;
}
void ThreadPoolExecutor::setRejectedExecutionHandler(
    const ::jxx::Ptr<RejectedExecutionHandler>& handler) {
    if (handler == nullptr) throw ::jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    handler_ = handler;
}
::jxx::Ptr<RejectedExecutionHandler> ThreadPoolExecutor::getRejectedExecutionHandler() {
    std::lock_guard<std::mutex> lock(mutex_);
    return handler_;
}
void ThreadPoolExecutor::reject_(const ::jxx::Ptr<::jxx::lang::Runnable>& command) {
    if (handler_ != nullptr) {
        handler_->rejectedExecution(command, ::jxx::CAST<ThreadPoolExecutor>(thisPtr()));
        return;
    }
    throw RejectedExecutionException();
}

void ThreadPoolExecutor::purge() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::deque<::jxx::Ptr<::jxx::lang::Runnable>> retained;
    while (!queue_.empty()) {
        const auto command = queue_.front();
        queue_.pop_front();
        const auto cancellable = ::jxx::CAST<internal::CancellableTask>(command);
        if (cancellable == nullptr || !cancellable->isCancelled()) {
            retained.push_back(command);
        }
    }
    queue_.swap(retained);
    if (shutdown_ && queue_.empty() && activeCount_ == 0) {
        workAvailable_.notify_all();
        terminated_.notify_all();
    }
}


} // namespace jxx::util::concurrent
