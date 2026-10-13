#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"

namespace jxx::util::concurrent::executors_detail {

class DelegatedExecutorService
    : public ::jxx::lang::ClassBase<
          DelegatedExecutorService,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::ExecutorService> {
public:
    explicit DelegatedExecutorService(
        const ::jxx::Ptr<::jxx::util::concurrent::ExecutorService>& executor);

    void execute(const ::jxx::Ptr<::jxx::lang::Runnable>& command) override;
    void shutdown() override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() override;
    ::jxx::lang::jbool isShutdown() override;
    ::jxx::lang::jbool isTerminated() override;
    ::jxx::lang::jbool awaitTermination(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;
    ::jxx::Ptr<::jxx::util::concurrent::wildcard::FutureAny> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) override;

protected:
    ::jxx::Ptr<::jxx::util::concurrent::ExecutorService> executor_;
};

class DelegatedScheduledExecutorService final
    : public ::jxx::lang::ClassBase<
          DelegatedScheduledExecutorService,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::ScheduledExecutorService> {
public:
    explicit DelegatedScheduledExecutorService(
        const ::jxx::Ptr<::jxx::util::concurrent::ScheduledExecutorService>& executor);

    void execute(const ::jxx::Ptr<::jxx::lang::Runnable>& command) override;
    void shutdown() override;
    ::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>> shutdownNow() override;
    ::jxx::lang::jbool isShutdown() override;
    ::jxx::lang::jbool isTerminated() override;
    ::jxx::lang::jbool awaitTermination(
        ::jxx::lang::jlong timeout,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;
    ::jxx::Ptr<::jxx::util::concurrent::wildcard::FutureAny> submit(
        const ::jxx::Ptr<::jxx::lang::Runnable>& task) override;
    void enqueueScheduledTask_(
        const ::jxx::Ptr<
            ::jxx::util::concurrent::scheduled_detail::ScheduledTask>& task) override;
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>> schedule(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>> scheduleAtFixedRate(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong period,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>> scheduleWithFixedDelay(
        const ::jxx::Ptr<::jxx::lang::Runnable>& command,
        ::jxx::lang::jlong initialDelay,
        ::jxx::lang::jlong delay,
        const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) override;

private:
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledExecutorService> scheduled_;
};

} // namespace jxx::util::concurrent::executors_detail
