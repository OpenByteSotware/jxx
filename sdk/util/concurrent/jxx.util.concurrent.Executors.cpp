#include <limits>
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.DelegatedExecutorService.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent
{
	jxx::Ptr<ExecutorService> Executors::newFixedThreadPool(jxx::lang::jint n)
	{
		if (n <= 0)throw jxx::lang::IllegalArgumentException(); return jxx::CAST<ExecutorService>(jxx::NEW<ThreadPoolExecutor>(n, n, 0, TimeUnit::MILLISECONDS()));
	}
	jxx::Ptr<ExecutorService> Executors::newFixedThreadPool(
        jxx::lang::jint n,
        const jxx::Ptr<ThreadFactory>& threadFactory)
    {
        if (n <= 0) throw jxx::lang::IllegalArgumentException();
        if (threadFactory == nullptr) throw jxx::lang::NullPointerException();
        return jxx::CAST<ExecutorService>(jxx::NEW<ThreadPoolExecutor>(
            n, n, 0, TimeUnit::MILLISECONDS(), threadFactory));
    }
	jxx::Ptr<ExecutorService> Executors::newSingleThreadExecutor()
	{
		return unconfigurableExecutorService(newFixedThreadPool(1));
	}
    jxx::Ptr<ExecutorService> Executors::newSingleThreadExecutor(
        const jxx::Ptr<ThreadFactory>& threadFactory)
    {
        return unconfigurableExecutorService(
            newFixedThreadPool(1, threadFactory));
    }
	jxx::Ptr<ExecutorService> Executors::newCachedThreadPool()
	{
        return newCachedThreadPool(defaultThreadFactory());
	}
    jxx::Ptr<ExecutorService> Executors::newCachedThreadPool(
        const jxx::Ptr<ThreadFactory>& threadFactory)
    {
        if (threadFactory == nullptr) throw jxx::lang::NullPointerException();
		return jxx::CAST<ExecutorService>(jxx::NEW<ThreadPoolExecutor>(
            0, std::numeric_limits<jxx::lang::jint>::max(),
            60, TimeUnit::SECONDS(), threadFactory));
    }
	jxx::Ptr<ScheduledExecutorService> Executors::newScheduledThreadPool(jxx::lang::jint corePoolSize)
	{
		if (corePoolSize < 0) throw jxx::lang::IllegalArgumentException();
		return jxx::CAST<ScheduledExecutorService>(jxx::NEW<ScheduledThreadPoolExecutor>(corePoolSize));
	}
    jxx::Ptr<ScheduledExecutorService> Executors::newScheduledThreadPool(
        jxx::lang::jint corePoolSize,
        const jxx::Ptr<ThreadFactory>& threadFactory)
    {
        if (corePoolSize < 0) throw jxx::lang::IllegalArgumentException();
        if (threadFactory == nullptr) throw jxx::lang::NullPointerException();
        return jxx::CAST<ScheduledExecutorService>(
            jxx::NEW<ScheduledThreadPoolExecutor>(corePoolSize, threadFactory));
    }
	jxx::Ptr<ScheduledExecutorService> Executors::newSingleThreadScheduledExecutor()
	{
		return unconfigurableScheduledExecutorService(
            newScheduledThreadPool(1));
	}
    jxx::Ptr<ScheduledExecutorService> Executors::newSingleThreadScheduledExecutor(
        const jxx::Ptr<ThreadFactory>& threadFactory)
    {
        return unconfigurableScheduledExecutorService(
            newScheduledThreadPool(1, threadFactory));
    }

    jxx::Ptr<ExecutorService> Executors::unconfigurableExecutorService(
        const jxx::Ptr<ExecutorService>& executor)
    {
        if (executor == nullptr) throw jxx::lang::NullPointerException();
        return jxx::CAST<ExecutorService>(
            jxx::NEW<executors_detail::DelegatedExecutorService>(executor));
    }

    jxx::Ptr<ScheduledExecutorService>
    Executors::unconfigurableScheduledExecutorService(
        const jxx::Ptr<ScheduledExecutorService>& executor)
    {
        if (executor == nullptr) throw jxx::lang::NullPointerException();
        return jxx::CAST<ScheduledExecutorService>(
            jxx::NEW<executors_detail::DelegatedScheduledExecutorService>(executor));
    }

	jxx::Ptr<jxx::lang::Object> Executors::cloneImpl() const
	{
		throw jxx::lang::CloneNotSupportedException();
	}
}
