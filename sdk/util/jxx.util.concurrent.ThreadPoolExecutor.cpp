#include "util/jxx.util.concurrent.ThreadPoolExecutor.h"

#include <algorithm>
#include <limits>

#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"

namespace jxx::util::concurrent {

ThreadPoolExecutor::ThreadPoolExecutor(
    jxx::lang::jint core,
    jxx::lang::jint maximum,
    jxx::lang::jlong keepAlive,
    const jxx::Ptr<TimeUnit>& unit)
    : corePoolSize_(core)
    , maximumPoolSize_(maximum)
    , keepAliveTime_(keepAlive)
    , keepAliveUnit_(unit) {
    if (core < 0 || maximum <= 0 || maximum < core || keepAlive < 0) {
        throw jxx::lang::IllegalArgumentException();
    }
    if (unit == nullptr) throw jxx::lang::NullPointerException();
}

ThreadPoolExecutor::~ThreadPoolExecutor() {
    shutdown();
    for (auto& worker : workers_) {
        if (worker.joinable()) worker.join();
    }
}

jxx::lang::jbool ThreadPoolExecutor::cachedMode_() const noexcept {
    return corePoolSize_ == 0 &&
        maximumPoolSize_ == std::numeric_limits<jxx::lang::jint>::max();
}

void ThreadPoolExecutor::startWorkerLocked_() {
    ++liveWorkerCount_;
    largestPoolSize_ = std::max(largestPoolSize_, liveWorkerCount_);
    workers_.emplace_back([this] { workerLoop_(); });
}

void ThreadPoolExecutor::execute(
    const jxx::Ptr<jxx::lang::Runnable>& command) {
    if (command == nullptr) throw jxx::lang::NullPointerException();
    {
        std::lock_guard<std::mutex> lock(mutex_);
        if (shutdown_) {
            reject_(command);
            return;
        }
        queue_.push_back(command);
        ++submittedTaskCount_;
        if (liveWorkerCount_ < corePoolSize_ ||
            (cachedMode_() && liveWorkerCount_ < maximumPoolSize_ &&
             activeCount_ + static_cast<jxx::lang::jint>(queue_.size()) > liveWorkerCount_)) {
            startWorkerLocked_();
        }
    }
    workAvailable_.notify_one();
}

void ThreadPoolExecutor::workerLoop_() {
    for (;;) {
        jxx::Ptr<jxx::lang::Runnable> task;
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

jxx::Ptr<jxx::util::List<jxx::lang::Runnable>>
ThreadPoolExecutor::shutdownNow() {
    auto result = jxx::NEW<jxx::util::ArrayList<jxx::lang::Runnable>>();
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
    return jxx::CAST<jxx::util::List<jxx::lang::Runnable>>(result);
}

jxx::lang::jbool ThreadPoolExecutor::isShutdown() {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_;
}

jxx::lang::jbool ThreadPoolExecutor::isTerminated() {
    std::lock_guard<std::mutex> lock(mutex_);
    return shutdown_ && queue_.empty() && activeCount_ == 0 && liveWorkerCount_ == 0;
}

jxx::lang::jbool ThreadPoolExecutor::awaitTermination(
    jxx::lang::jlong timeout,
    const jxx::Ptr<TimeUnit>& unit) {
    if (unit == nullptr) throw jxx::lang::NullPointerException();
    std::unique_lock<std::mutex> lock(mutex_);
    return terminated_.wait_for(lock, unit->toChrono(timeout), [&] {
        return shutdown_ && queue_.empty() && activeCount_ == 0 && liveWorkerCount_ == 0;
    });
}

jxx::lang::jint ThreadPoolExecutor::getCorePoolSize() const noexcept { return corePoolSize_; }
jxx::lang::jint ThreadPoolExecutor::getMaximumPoolSize() const noexcept { return maximumPoolSize_; }
jxx::lang::jlong ThreadPoolExecutor::getKeepAliveTime(const jxx::Ptr<TimeUnit>& unit) const {
    if (unit == nullptr) throw jxx::lang::NullPointerException();
    return unit->convert(keepAliveTime_, keepAliveUnit_);
}
jxx::lang::jint ThreadPoolExecutor::getPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return liveWorkerCount_;
}
jxx::lang::jint ThreadPoolExecutor::getActiveCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return activeCount_;
}
jxx::lang::jint ThreadPoolExecutor::getLargestPoolSize() {
    std::lock_guard<std::mutex> lock(mutex_);
    return largestPoolSize_;
}
jxx::lang::jlong ThreadPoolExecutor::getCompletedTaskCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return completedTaskCount_;
}
jxx::lang::jlong ThreadPoolExecutor::getTaskCount() {
    std::lock_guard<std::mutex> lock(mutex_);
    return submittedTaskCount_;
}
void ThreadPoolExecutor::setRejectedExecutionHandler(
    const jxx::Ptr<RejectedExecutionHandler>& handler) {
    if (handler == nullptr) throw jxx::lang::NullPointerException();
    std::lock_guard<std::mutex> lock(mutex_);
    handler_ = handler;
}
jxx::Ptr<RejectedExecutionHandler> ThreadPoolExecutor::getRejectedExecutionHandler() {
    std::lock_guard<std::mutex> lock(mutex_);
    return handler_;
}
void ThreadPoolExecutor::reject_(const jxx::Ptr<jxx::lang::Runnable>& command) {
    if (handler_ != nullptr) {
        handler_->rejectedExecution(command, jxx::CAST<ThreadPoolExecutor>(thisPtr()));
        return;
    }
    throw RejectedExecutionException();
}

} // namespace jxx::util::concurrent
