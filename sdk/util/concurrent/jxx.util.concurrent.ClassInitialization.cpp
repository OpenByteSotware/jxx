#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "util/concurrent/jxx.util.concurrent.CountDownLatch.h"
#include "util/concurrent/jxx.util.concurrent.CyclicBarrier.h"
#include "util/concurrent/jxx.util.concurrent.DelegatedExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.Delayed.h"
#include "util/concurrent/jxx.util.concurrent.Executor.h"
#include "util/concurrent/jxx.util.concurrent.ExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinPool.h"
#include "util/concurrent/jxx.util.concurrent.FutureAnyTask.h"
#include "util/concurrent/jxx.util.concurrent.Phaser.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionHandler.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.Semaphore.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "util/concurrent/jxx.util.concurrent.wildcard.FutureAny.h"
#include "util/concurrent/jxx.util.concurrent.AbstractExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.BrokenBarrierException.h"
#include "util/concurrent/jxx.util.concurrent.CancellableTask.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.CompletionException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinWorkerThread.h"
#include "util/concurrent/jxx.util.concurrent.RecursiveAction.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureAnyAdapter.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledTask.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"
#include "util/concurrent/jxx.util.concurrent.ThreadLocalRandom.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeoutException.h"

namespace jxx::util::concurrent {
namespace {
template <typename T>
void registerType()
{
    ::jxx::lang::class_info_detail::registerClassInfo<T>();
}
} // namespace

void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        registerType<CountDownLatch>();
        registerType<CyclicBarrier>();
        registerType<executors_detail::DefaultThreadFactory>();
        registerType<executors_detail::DelegatedExecutorService>();
        registerType<executors_detail::DelegatedScheduledExecutorService>();
        registerType<Delayed>();
        registerType<Executor>();
        registerType<ExecutorService>();
        registerType<Executors>();
        registerType<ForkJoinPool>();
        registerType<FutureAnyTask>();
        registerType<Phaser>();
        registerType<RejectedExecutionHandler>();
        registerType<ScheduledExecutorService>();
        registerType<ScheduledThreadPoolExecutor>();
        registerType<Semaphore>();
        registerType<TimeUnit>();
        registerType<wildcard::FutureAny>();
        registerType<AbstractExecutorService>();
        registerType<BrokenBarrierException>();
        registerType<::jxx::util::concurrent::internal::CancellableTask>();
        registerType<CancellationException>();
        registerType<CompletionException>();
        registerType<ExecutionException>();
        registerType<ForkJoinWorkerThread>();
        registerType<RecursiveAction>();
        registerType<RejectedExecutionException>();
        registerType<::jxx::util::concurrent::scheduled_detail::ScheduledFutureAnyAdapter>();
        registerType<::jxx::util::concurrent::scheduled_detail::ScheduledTask>();
        registerType<ThreadFactory>();
        registerType<ThreadLocalRandom>();
        registerType<ThreadPoolExecutor>();
        registerType<TimeoutException>();
    });
}

} // namespace jxx::util::concurrent
