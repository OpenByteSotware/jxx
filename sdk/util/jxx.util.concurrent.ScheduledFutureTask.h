#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.RunnableScheduledFuture.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {

template<typename V>
class ScheduledFutureTask final
    : public ::jxx::lang::ClassBase<ScheduledFutureTask<V>,
          ::jxx::lang::Object, RunnableScheduledFuture<V>> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<ScheduledFutureTask<V>,
        JxxSuper, RunnableScheduledFuture<V>>;
    using JxxClassInfoMarker = typename Super::JxxClassInfoMarker;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class() { return JxxClassInfoMarker::Class(); }

    ScheduledFutureTask(const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        const ::jxx::Ptr<V>& result, ::jxx::lang::jlong delayNanos,
        ::jxx::lang::jlong periodNanos = 0)
        : Super(), command_(command), result_(result),
          trigger_(clock::now()+std::chrono::nanoseconds(delayNanos)),
          periodNanos_(periodNanos), sequence_(nextSequence_++) {
        if(!command_) throw ::jxx::lang::NullPointerException();
    }

    ScheduledFutureTask(
        const ::jxx::Ptr<Callable<V>>& callable,
        ::jxx::lang::jlong delayNanos)
        : Super(), callable_(callable),
          trigger_(clock::now()+std::chrono::nanoseconds(delayNanos)),
          periodNanos_(0), sequence_(nextSequence_++) {
        if (callable_ == nullptr) throw ::jxx::lang::NullPointerException();
    }

    void run() override {
        { std::lock_guard<std::mutex> l(mutex_); if(cancelled_||done_||running_) return; running_=true; }
        try {
            if (callable_ != nullptr) result_ = callable_->call();
            else command_->run();
            finishRun_(nullptr);
        }
        catch(const ::jxx::lang::Throwable& e) { finishRun_(e.cloneThrowable()); }
        catch(...) { finishRun_(::jxx::NEW<::jxx::lang::RuntimeException>()); }
    }

    ::jxx::lang::jbool cancel(::jxx::lang::jbool) override {
        std::lock_guard<std::mutex> l(mutex_); if(done_) return false;
        cancelled_=true; done_=true; condition_.notify_all(); return true;
    }
    ::jxx::lang::jbool isCancelled() override { std::lock_guard<std::mutex> l(mutex_); return cancelled_; }
    ::jxx::lang::jbool isDone() override { std::lock_guard<std::mutex> l(mutex_); return done_; }
    ::jxx::lang::jbool isPeriodic() override { return periodNanos_!=0; }
    ::jxx::Ptr<V> get() override { std::unique_lock<std::mutex> l(mutex_); condition_.wait(l,[&]{return done_;}); return report_(); }
    ::jxx::Ptr<V> get(::jxx::lang::jlong timeout,const ::jxx::Ptr<TimeUnit>& unit) override {
        if(!unit) throw ::jxx::lang::NullPointerException(); std::unique_lock<std::mutex> l(mutex_);
        if(!condition_.wait_for(l,unit->toChrono(timeout),[&]{return done_;})) throw TimeoutException(); return report_();
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
        const auto difference=getDelay(TimeUnit::NANOSECONDS())-other->getDelay(TimeUnit::NANOSECONDS()); return difference<0?-1:difference>0?1:0;
    }

private:
    using clock=std::chrono::steady_clock;
    void finishRun_(const ::jxx::Ptr<::jxx::lang::Throwable>& failure) {
        std::lock_guard<std::mutex> l(mutex_); running_=false;
        if(cancelled_) return;
        if(failure){failure_=failure;done_=true;condition_.notify_all();return;}
        if(periodNanos_==0){done_=true;condition_.notify_all();return;}
        if(periodNanos_>0) trigger_+=std::chrono::nanoseconds(periodNanos_);
        else trigger_=clock::now()+std::chrono::nanoseconds(-periodNanos_);
    }
    ::jxx::Ptr<V> report_(){if(cancelled_)throw CancellationException();if(failure_)throw ExecutionException(failure_);return result_;}
    inline static std::atomic<::jxx::lang::jlong> nextSequence_{0};
    ::jxx::Ptr<::jxx::lang::Runnable> command_;
    ::jxx::Ptr<Callable<V>> callable_;
    ::jxx::Ptr<V> result_;
    clock::time_point trigger_; ::jxx::lang::jlong periodNanos_; ::jxx::lang::jlong sequence_;
    mutable std::mutex mutex_; std::condition_variable condition_;
    ::jxx::lang::jbool running_=false,done_=false,cancelled_=false;
    ::jxx::Ptr<::jxx::lang::Throwable> failure_;
};

} // namespace jxx::util::concurrent
