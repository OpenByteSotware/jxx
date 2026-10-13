#include "util/concurrent/jxx.util.concurrent.ForkJoinPool.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/concurrent/jxx.util.concurrent.FutureTask.h"

namespace jxx::util::concurrent {

ForkJoinPool::ForkJoinPool(::jxx::lang::jint parallelism)
    : parallelism_(parallelism), common_(false) {
    if (parallelism <= 0) throw ::jxx::lang::IllegalArgumentException();
    executor_ = ::jxx::NEW<ThreadPoolExecutor>(
        parallelism, parallelism, 0, TimeUnit::MILLISECONDS());
}

::jxx::Ptr<ForkJoinPool> ForkJoinPool::commonPool() {
    static const auto pool = [] {
        auto value = ::jxx::NEW<ForkJoinPool>(
            std::max<::jxx::lang::jint>(1,
                static_cast<::jxx::lang::jint>(std::thread::hardware_concurrency())));
        value->common_ = true;
        return value;
    }();
    return pool;
}

::jxx::lang::jint ForkJoinPool::getParallelism() const noexcept {
    return parallelism_;
}

void ForkJoinPool::execute(
    const ::jxx::Ptr<::jxx::lang::Runnable>& command) {
    executor_->execute(command);
}

void ForkJoinPool::shutdown() {
    if (!common_) executor_->shutdown();
}

::jxx::Ptr<::jxx::util::List<::jxx::lang::Runnable>>
ForkJoinPool::shutdownNow() {
    if (common_) return ::jxx::NEW<::jxx::util::ArrayList<::jxx::lang::Runnable>>();
    return executor_->shutdownNow();
}

::jxx::lang::jbool ForkJoinPool::isShutdown() {
    return common_ ? false : executor_->isShutdown();
}

::jxx::lang::jbool ForkJoinPool::isTerminated() {
    return common_ ? false : executor_->isTerminated();
}

::jxx::lang::jbool ForkJoinPool::awaitTermination(
    ::jxx::lang::jlong timeout, const ::jxx::Ptr<TimeUnit>& unit) {
    if (common_) {
        if (unit == nullptr) throw ::jxx::lang::NullPointerException();
        return false;
    }
    return executor_->awaitTermination(timeout, unit);
}

::jxx::Ptr<wildcard::FutureAny> ForkJoinPool::submit(
    const ::jxx::Ptr<::jxx::lang::Runnable>& task) {
    return executor_->submit(task);
}

} // namespace jxx::util::concurrent
