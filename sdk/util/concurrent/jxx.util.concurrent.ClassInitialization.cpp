#include "util/concurrent/jxx.util.concurrent.ClassInitialization.h"
#include "util/concurrent/atomic/jxx.util.concurrent.atomic.ClassInitialization.h"
#include "util/concurrent/locks/jxx.util.concurrent.locks.ClassInitialization.h"

#include <mutex>

#include "lang/jxx.lang.ClassInfo.h"
#include "io/jxx.io.ArrayCodecRegistry.h"
#include "io/jxx.io.SerializationClassRegistry.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedQueue.h"
#include "util/concurrent/jxx.util.concurrent.ArrayBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedBlockingDeque.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinTask.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArrayList.h"
#include "util/concurrent/jxx.util.concurrent.CopyOnWriteArraySet.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentLinkedDeque.h"
#include "util/concurrent/jxx.util.concurrent.PriorityBlockingQueue.h"
#include "util/concurrent/jxx.util.concurrent.DelayQueue.h"
#include "util/concurrent/jxx.util.concurrent.LinkedTransferQueue.h"
#include "util/concurrent/jxx.util.concurrent.SynchronousQueue.h"
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
        using ObjectType = ::jxx::lang::Object;
        using ErasedCopyOnWriteArrayList = CopyOnWriteArrayList<ObjectType>;
        registerType<ErasedCopyOnWriteArrayList>();
        ::jxx::io::serialization::registerSerializableClass<
            ErasedCopyOnWriteArrayList>(
                static_cast<::jxx::lang::jlong>(8673264195747942595LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ErasedCopyOnWriteArrayList, 1U, 2U>();

        using ErasedCopyOnWriteArraySet = CopyOnWriteArraySet<ObjectType>;
        registerType<ErasedCopyOnWriteArraySet>();
        ::jxx::io::serialization::registerSerializableClass<
            ErasedCopyOnWriteArraySet>(
                static_cast<::jxx::lang::jlong>(5457747651344034263LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            ErasedCopyOnWriteArraySet, 1U, 2U>();
        using ErasedConcurrentLinkedQueue = ConcurrentLinkedQueue<ObjectType>;
        registerType<ErasedConcurrentLinkedQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedConcurrentLinkedQueue>(
            static_cast<::jxx::lang::jlong>(196745693267521676LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedConcurrentLinkedQueue, 1U, 2U>();

        using ErasedConcurrentLinkedDeque = ConcurrentLinkedDeque<ObjectType>;
        registerType<ErasedConcurrentLinkedDeque>();
        ::jxx::io::serialization::registerSerializableClass<ErasedConcurrentLinkedDeque>(
            static_cast<::jxx::lang::jlong>(876323262645176354LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedConcurrentLinkedDeque, 1U, 2U>();

        using ErasedPriorityBlockingQueue = PriorityBlockingQueue<ObjectType>;
        registerType<ErasedPriorityBlockingQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedPriorityBlockingQueue>(
            static_cast<::jxx::lang::jlong>(5595510919245408276LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedPriorityBlockingQueue, 1U, 2U>();

        using ErasedDelayQueue = DelayQueue<ObjectType>;
        registerType<ErasedDelayQueue>();
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedDelayQueue, 1U, 2U>();

        using ErasedLinkedTransferQueue = LinkedTransferQueue<ObjectType>;
        registerType<ErasedLinkedTransferQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedTransferQueue>(
            static_cast<::jxx::lang::jlong>(-3223113410248163686LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedTransferQueue, 1U, 2U>();

        using ErasedSynchronousQueue = SynchronousQueue<ObjectType>;
        registerType<ErasedSynchronousQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedSynchronousQueue>(
            static_cast<::jxx::lang::jlong>(-3223113410248163686LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedSynchronousQueue, 1U, 2U>();
        using ErasedConcurrentHashMap = ConcurrentHashMap<ObjectType, ObjectType>;
        registerType<ErasedConcurrentHashMap>();
        ::jxx::io::serialization::registerSerializableClass<ErasedConcurrentHashMap>(
            static_cast<::jxx::lang::jlong>(7249069246763182397LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedConcurrentHashMap, 1U, 2U>();

        using ErasedArrayBlockingQueue = ArrayBlockingQueue<ObjectType>;
        registerType<ErasedArrayBlockingQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedArrayBlockingQueue>(
            static_cast<::jxx::lang::jlong>(-817911632652898426LL),
            [] { return ::jxx::NEW<ErasedArrayBlockingQueue>(1); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedArrayBlockingQueue, 1U, 2U>();

        using ErasedLinkedBlockingQueue = LinkedBlockingQueue<ObjectType>;
        registerType<ErasedLinkedBlockingQueue>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedBlockingQueue>(
            static_cast<::jxx::lang::jlong>(-6903933977591709194LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedBlockingQueue, 1U, 2U>();

        using ErasedLinkedBlockingDeque = LinkedBlockingDeque<ObjectType>;
        registerType<ErasedLinkedBlockingDeque>();
        ::jxx::io::serialization::registerSerializableClass<ErasedLinkedBlockingDeque>(
            static_cast<::jxx::lang::jlong>(-387911632671998426LL));
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedLinkedBlockingDeque, 1U, 2U>();

        using ErasedForkJoinTask = ForkJoinTask<ObjectType>;
        registerType<ErasedForkJoinTask>();
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<ErasedForkJoinTask, 1U, 2U>();

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
        ::jxx::io::serialization::registerSerializableClass<Semaphore>(
            1,
            [] { return ::jxx::NEW<Semaphore>(0, false); });
        ::jxx::io::serialization::registerClassReferenceArrayCodecs<
            Semaphore, 1U, 2U>();
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


void initialize()
{
    initializeClasses();
    ::jxx::util::concurrent::atomic::initialize();
    ::jxx::util::concurrent::locks::initialize();
}

} // namespace jxx::util::concurrent
