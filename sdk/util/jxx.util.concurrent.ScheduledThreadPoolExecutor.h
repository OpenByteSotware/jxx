#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "util/jxx.util.concurrent.RunnableAdapter.h"
#include "util/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/jxx.util.concurrent.ThreadPoolExecutor.h"

namespace jxx::util::concurrent {

class ScheduledThreadPoolExecutor
    : public ::jxx::lang::ClassBase<ScheduledThreadPoolExecutor,
          ThreadPoolExecutor, ScheduledExecutorService> {
public:
    using JxxSuper=ThreadPoolExecutor;
    using Super=::jxx::lang::ClassBase<ScheduledThreadPoolExecutor,JxxSuper,ScheduledExecutorService>;
    using JxxClassInfoMarker=::jxx::lang::ClassInfo<ScheduledThreadPoolExecutor,JxxSuper,ScheduledExecutorService>;
    static ::jxx::Ptr<::jxx::lang::ClassAny> Class();

    explicit ScheduledThreadPoolExecutor(::jxx::lang::jint corePoolSize);
    ~ScheduledThreadPoolExecutor() override;

    ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> schedule(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,::jxx::lang::jlong delay,
        const ::jxx::Ptr<TimeUnit>& unit) override;
    ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> scheduleAtFixedRate(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong period,const ::jxx::Ptr<TimeUnit>& unit) override;
    ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> scheduleWithFixedDelay(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong delay,const ::jxx::Ptr<TimeUnit>& unit) override;
    void execute(const ::jxx::Ptr<::jxx::lang::Runnable>& command) override;
    void shutdown() override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() override;

private:
    using Task=ScheduledFutureTask<::jxx::lang::Object>;
    struct Later { bool operator()(const ::jxx::Ptr<Task>& a,const ::jxx::Ptr<Task>& b) const { return a->compareTo(::jxx::CAST<Delayed>(b))>0; } };
    ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> schedule_(const ::jxx::Ptr<::jxx::lang::Runnable>& command,::jxx::lang::jlong delayNanos,::jxx::lang::jlong periodNanos);
    void dispatch_();
    std::mutex scheduleMutex_; std::condition_variable scheduleChanged_;
    std::priority_queue<::jxx::Ptr<Task>,std::vector<::jxx::Ptr<Task>>,Later> scheduled_;
    std::thread dispatcher_; ::jxx::lang::jbool stopping_=false;
};

} // namespace jxx::util::concurrent
