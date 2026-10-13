#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.RunnableScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

template<typename V>
class ScheduledFutureTask final
    : public ::jxx::lang::ClassBase<ScheduledFutureTask<V>,
          ::jxx::lang::Object,
          RunnableScheduledFuture<V>,
          scheduled_detail::ScheduledTask> {
public:
    enum class PeriodicMode {
        OneShot,
        FixedRate,
        FixedDelay
    };

    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ScheduledFutureTask<V>,
        JxxSuper,
        RunnableScheduledFuture<V>,
        scheduled_detail::ScheduledTask>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() { return JxxClassInfoMarker::Class(); }

    ScheduledFutureTask(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        const ::jxx::Ptr<V>& result,
        ::jxx::lang::jlong delayNanos,
        ::jxx::lang::jlong periodNanos = 0)
        : ScheduledFutureTask(
              command,
              result,
              delayNanos,
              periodNanos,
              periodNanos > 0
                  ? PeriodicMode::FixedRate
                  : PeriodicMode::OneShot) {}

    ScheduledFutureTask(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        const ::jxx::Ptr<V>& result,
        ::jxx::lang::jlong delayNanos,
        ::jxx::lang::jlong periodNanos,
        PeriodicMode periodicMode)
        : Super(), command_(command), result_(result),
          trigger_(deadlineAfter_(delayNanos)),
          periodNanos_(periodNanos), periodicMode_(periodicMode),
          sequence_(nextSequence_++) {
        if (command_ == nullptr) throw ::jxx::lang::NullPointerException();
        if (periodicMode_ == PeriodicMode::OneShot) periodNanos_ = 0;
        if (periodicMode_ != PeriodicMode::OneShot && periodNanos_ <= 0) {
            throw ::jxx::lang::IllegalArgumentException();
        }
    }

    ScheduledFutureTask(
        const ::jxx::Ptr<Callable<V>>& callable,
        ::jxx::lang::jlong delayNanos)
        : Super(), callable_(callable),
          trigger_(deadlineAfter_(delayNanos)),
          periodNanos_(0), sequence_(nextSequence_++) {
        if (callable_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    void run() override {
        {
            std::lock_guard<std::mutex> l(mutex_);
            if (cancelled_ || done_ || running_) return;
            running_ = true;
            runner_ = ::jxx::lang::Thread::currentThread();
        }
        try {
            if (callable_ != nullptr) result_ = callable_->call();
            else command_->run();
            finishRun_(nullptr);
        }
        catch (const ::jxx::lang::Throwable& failure) {
            finishRun_(failure.cloneThrowable());
        }
        catch (...) {
            finishRun_(::jxx::NEW<::jxx::lang::RuntimeException>());
        }
    }

    ::jxx::lang::jbool cancel(
        ::jxx::lang::jbool mayInterruptIfRunning) override {
        ::jxx::Ptr<::jxx::lang::Thread> runner;
        {
            std::lock_guard<std::mutex> l(mutex_);
            if (done_) return false;
            cancelled_ = true;
            done_ = true;
            if (mayInterruptIfRunning && running_) runner = runner_.lock();
            condition_.notify_all();
        }
        if (runner != nullptr) runner->interrupt();
        std::function<void()> callback;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            callback = cancelCallback_;
        }
        if (callback) callback();
        return true;
    }

    void setCancelCallback(std::function<void()> callback) override {
        std::lock_guard<std::mutex> lock(mutex_);
        cancelCallback_ = std::move(callback);
    }
    ::jxx::lang::jbool isCancelled() override { std::lock_guard<std::mutex> l(mutex_); return cancelled_; }
    ::jxx::lang::jbool isDone() override { std::lock_guard<std::mutex> l(mutex_); return done_; }
    ::jxx::lang::jbool isPeriodic() override {
        return periodicMode_ != PeriodicMode::OneShot;
    }

    ::jxx::lang::jbool isPeriodic() const noexcept override {
        return periodicMode_ != PeriodicMode::OneShot;
    }
    ::jxx::Ptr<V> get() override {
        const auto current = ::jxx::lang::Thread::currentThread();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }

        std::unique_lock<std::mutex> lock(mutex_);
        if (!done_) {
            current->setParkWakeup_([this] { condition_.notify_all(); });
            try {
                condition_.wait(lock, [this, &current] {
                    return done_ || current->isInterrupted();
                });
                current->clearParkWakeup_();
            } catch (...) {
                current->clearParkWakeup_();
                throw;
            }
        }
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }
        return report_();
    }

    ::jxx::Ptr<V> get(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<TimeUnit>& unit) override {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        const auto current = ::jxx::lang::Thread::currentThread();
        if (::jxx::lang::Thread::interrupted()) {
            throw ::jxx::lang::InterruptedException();
        }

        std::unique_lock<std::mutex> lock(mutex_);
        if (done_) return report_();
        if (timeout <= 0) throw TimeoutException();

        const auto duration = unit->toChrono(timeout);
        const auto now = std::chrono::steady_clock::now();
        const auto remaining = std::chrono::steady_clock::time_point::max() - now;
        const auto deadline = duration >= remaining
            ? std::chrono::steady_clock::time_point::max()
            : now + duration;

        current->setParkWakeup_([this] { condition_.notify_all(); });
        try {
            const auto completed = condition_.wait_until(
                lock,
                deadline,
                [this, &current] {
                    return done_ || current->isInterrupted();
                });
            current->clearParkWakeup_();
            if (::jxx::lang::Thread::interrupted()) {
                throw ::jxx::lang::InterruptedException();
            }
            if (!completed || !done_) throw TimeoutException();
        } catch (...) {
            current->clearParkWakeup_();
            throw;
        }
        return report_();
    }
    ::jxx::lang::jlong getDelay(const ::jxx::Ptr<TimeUnit>& unit) const override {
        if(!unit) throw ::jxx::lang::NullPointerException();
        auto nanos=std::chrono::duration_cast<std::chrono::nanoseconds>(trigger_-clock::now()).count();
        return unit->convert(nanos,TimeUnit::NANOSECONDS());
    }
    ::jxx::lang::jint compareTo(const ::jxx::Ptr<Delayed>& other) const override {
        if(!other) throw ::jxx::lang::NullPointerException();
        auto task=::jxx::CAST<ScheduledFutureTask<V>>(other);
        if(task){if(trigger_<task->trigger_)return -1;if(trigger_>task->trigger_)return 1;return sequence_<task->sequence_?-1:sequence_>task->sequence_;}
        const auto thisDelay = getDelay(TimeUnit::NANOSECONDS());
        const auto otherDelay = other->getDelay(TimeUnit::NANOSECONDS());
        if (thisDelay < otherDelay) return -1;
        if (thisDelay > otherDelay) return 1;
        return 0;
    }

private:
    using clock=std::chrono::steady_clock;

    static clock::time_point addSaturated_(
        const clock::time_point& base,
        ::jxx::lang::jlong nanos) noexcept {
        if (nanos <= 0) return base;
        const auto duration = std::chrono::nanoseconds(nanos);
        const auto remaining = clock::time_point::max() - base;
        if (duration >= remaining) return clock::time_point::max();
        return base + duration;
    }

    static clock::time_point deadlineAfter_(
        ::jxx::lang::jlong delayNanos) noexcept {
        return addSaturated_(clock::now(), delayNanos);
    }
    void finishRun_(const ::jxx::Ptr<::jxx::lang::Throwable>& failure) {
        std::lock_guard<std::mutex> l(mutex_);
        running_ = false;
        runner_.reset();
        if(cancelled_) return;
        if(failure){failure_=failure;done_=true;condition_.notify_all();return;}
        if (periodicMode_ == PeriodicMode::OneShot) {
            done_ = true;
            condition_.notify_all();
            return;
        }
        if (periodicMode_ == PeriodicMode::FixedRate) {
            trigger_ = addSaturated_(trigger_, periodNanos_);
        } else {
            trigger_ = deadlineAfter_(periodNanos_);
        }
    }
    ::jxx::Ptr<V> report_(){if(cancelled_)throw CancellationException();if(failure_)throw ExecutionException(failure_);return result_;}
    inline static std::atomic<::jxx::lang::jlong> nextSequence_{0};
    ::jxx::Ptr<::jxx::lang::Runnable> command_;
    ::jxx::Ptr<Callable<V>> callable_;
    ::jxx::Ptr<V> result_;
    clock::time_point trigger_;
    ::jxx::lang::jlong periodNanos_;
    PeriodicMode periodicMode_ = PeriodicMode::OneShot;
    ::jxx::lang::jlong sequence_;
    mutable std::mutex mutex_; std::condition_variable condition_;
    ::jxx::lang::jbool running_=false,done_=false,cancelled_=false;
    std::weak_ptr<::jxx::lang::Thread> runner_;
    ::jxx::Ptr<::jxx::lang::Throwable> failure_;
    std::function<void()> cancelCallback_;
};

} // namespace jxx::util::concurrent
