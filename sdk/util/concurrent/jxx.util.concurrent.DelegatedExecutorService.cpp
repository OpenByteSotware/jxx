#include "util/concurrent/jxx.util.concurrent.DelegatedExecutorService.h"

namespace jxx::util::concurrent::executors_detail {

DelegatedExecutorService::DelegatedExecutorService(
    const ::jxx::Ptr<::jxx::util::concurrent::ExecutorService>& executor)
    : executor_(executor) {
    if (executor_ == nullptr) throw ::jxx::lang::NullPointerException();
}

void DelegatedExecutorService::execute(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command) {
    executor_->execute(command);
}
void DelegatedExecutorService::shutdown() { executor_->shutdown(); }
::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>>
DelegatedExecutorService::shutdownNow() { return executor_->shutdownNow(); }
::jxx::lang::jbool DelegatedExecutorService::isShutdown() { return executor_->isShutdown(); }
::jxx::lang::jbool DelegatedExecutorService::isTerminated() { return executor_->isTerminated(); }
::jxx::lang::jbool DelegatedExecutorService::awaitTermination(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return executor_->awaitTermination(timeout, unit);
}
::jxx::Ptr<::jxx::util::concurrent::wildcard::FutureAny>
DelegatedExecutorService::submit(const ::jxx::Ptr<::jxx::lang::Runnable>& task) {
    return executor_->submit(task);
}

DelegatedScheduledExecutorService::DelegatedScheduledExecutorService(
    const ::jxx::Ptr<::jxx::util::concurrent::ScheduledExecutorService>& executor)
    : scheduled_(executor) {
    if (scheduled_ == nullptr) throw ::jxx::lang::NullPointerException();
}

void DelegatedScheduledExecutorService::execute(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command) {
    scheduled_->execute(command);
}
void DelegatedScheduledExecutorService::shutdown() { scheduled_->shutdown(); }
::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>>
DelegatedScheduledExecutorService::shutdownNow() { return scheduled_->shutdownNow(); }
::jxx::lang::jbool DelegatedScheduledExecutorService::isShutdown() { return scheduled_->isShutdown(); }
::jxx::lang::jbool DelegatedScheduledExecutorService::isTerminated() { return scheduled_->isTerminated(); }
::jxx::lang::jbool DelegatedScheduledExecutorService::awaitTermination(
    ::jxx::lang::jlong timeout,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return scheduled_->awaitTermination(timeout, unit);
}
::jxx::Ptr<::jxx::util::concurrent::wildcard::FutureAny>
DelegatedScheduledExecutorService::submit(
    const ::jxx::Ptr<::jxx::lang::Runnable>& task) {
    return scheduled_->submit(task);
}

::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>>
DelegatedScheduledExecutorService::schedule(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command,
    ::jxx::lang::jlong delay,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return scheduled_->schedule(command, delay, unit);
}
::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>>
DelegatedScheduledExecutorService::scheduleAtFixedRate(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command,
    ::jxx::lang::jlong initialDelay,
    ::jxx::lang::jlong period,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return scheduled_->scheduleAtFixedRate(command, initialDelay, period, unit);
}
::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<::jxx::lang::Object>>
DelegatedScheduledExecutorService::scheduleWithFixedDelay(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command,
    ::jxx::lang::jlong initialDelay,
    ::jxx::lang::jlong delay,
    const ::jxx::Ptr<::jxx::util::concurrent::TimeUnit>& unit) {
    return scheduled_->scheduleWithFixedDelay(command, initialDelay, delay, unit);
}

} // namespace jxx::util::concurrent::executors_detail
