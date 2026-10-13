#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#include "util/concurrent/jxx.util.concurrent.RunnableAdapter.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"

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
    ScheduledThreadPoolExecutor(
        ::jxx::lang::jint corePoolSize,
        const ::jxx::Ptr<ThreadFactory>& threadFactory);
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
    ::jxx::Ptr<wildcard::FutureAny> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) override;
    void enqueueScheduledTask_(
        const ::jxx::Ptr<scheduled_detail::ScheduledTask>& task) override;
    void shutdown() override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() override;
    ::jxx::lang::jbool isShutdown() override;

    void setContinueExistingPeriodicTasksAfterShutdownPolicy(
        ::jxx::lang::jbool value);
    ::jxx::lang::jbool
    getContinueExistingPeriodicTasksAfterShutdownPolicy() const;
    void setExecuteExistingDelayedTasksAfterShutdownPolicy(
        ::jxx::lang::jbool value);
    ::jxx::lang::jbool
    getExecuteExistingDelayedTasksAfterShutdownPolicy() const;
    void setRemoveOnCancelPolicy(::jxx::lang::jbool value);
    ::jxx::lang::jbool getRemoveOnCancelPolicy() const;

private:
    using Task = scheduled_detail::ScheduledTask;
    using ConcreteTask = ScheduledFutureTask<::jxx::lang::Object>;

    class ScheduledTaskRunner final
        : public ::jxx::lang::ClassBase<
              ScheduledTaskRunner,
              ::jxx::lang::Object,
              ::jxx::lang::Runnable> {
    public:
        ScheduledTaskRunner(
            ScheduledThreadPoolExecutor* owner,
            const ::jxx::Ptr<Task>& task)
            : owner_(owner), task_(task) {}

        void run() override {
            task_->run();
            owner_->afterExecute_(task_);
        }

    private:
        ScheduledThreadPoolExecutor* owner_;
        ::jxx::Ptr<Task> task_;
    };
    struct Later { bool operator()(const ::jxx::Ptr<Task>& a,const ::jxx::Ptr<Task>& b) const { return a->getDelay(TimeUnit::NANOSECONDS()) > b->getDelay(TimeUnit::NANOSECONDS()); } };
    ::jxx::Ptr<ScheduledFuture<::jxx::lang::Object>> schedule_(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong delayNanos,
        ::jxx::lang::jlong periodNanos,
        typename ConcreteTask::PeriodicMode periodicMode);
    void dispatch_();
    void afterExecute_(const ::jxx::Ptr<Task>& task);
    void removeCancelledTask_(const ::jxx::Ptr<Task>& task);
    mutable std::mutex scheduleMutex_; std::condition_variable scheduleChanged_;
    std::priority_queue<::jxx::Ptr<Task>,std::vector<::jxx::Ptr<Task>>,Later> scheduled_;
    std::thread dispatcher_;
    ::jxx::lang::jbool stopping_=false;
    std::atomic<::jxx::lang::jbool> shutdownRequested_{false};
    ::jxx::lang::jbool continuePeriodicAfterShutdown_=false;
    ::jxx::lang::jbool executeDelayedAfterShutdown_=true;
    ::jxx::lang::jbool removeOnCancel_=false;
};

} // namespace jxx::util::concurrent
