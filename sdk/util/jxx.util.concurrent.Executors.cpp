#include <limits>
#include "util/jxx.util.concurrent.Executors.h"
#include "lang/jxx.lang.Exceptions.h"
#include "util/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "util/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace jxx::util::concurrent
{
	jxx::Ptr<ExecutorService> Executors::newFixedThreadPool(jxx::lang::jint n)
	{
		if (n <= 0)throw jxx::lang::IllegalArgumentException(); return jxx::CAST<ExecutorService>(jxx::NEW<ThreadPoolExecutor>(n, n, 0, TimeUnit::MILLISECONDS()));
	}
	jxx::Ptr<ExecutorService> Executors::newSingleThreadExecutor()
	{
		return newFixedThreadPool(1);
	}
	jxx::Ptr<ExecutorService> Executors::newCachedThreadPool()
	{
		return jxx::CAST<ExecutorService>(jxx::NEW<ThreadPoolExecutor>(
            0, std::numeric_limits<jxx::lang::jint>::max(),
            60, TimeUnit::SECONDS()));
	}
	jxx::Ptr<ScheduledExecutorService> Executors::newScheduledThreadPool(jxx::lang::jint corePoolSize)
	{
		if (corePoolSize <= 0) throw jxx::lang::IllegalArgumentException();
		return jxx::CAST<ScheduledExecutorService>(jxx::NEW<ScheduledThreadPoolExecutor>(corePoolSize));
	}
	jxx::Ptr<ScheduledExecutorService> Executors::newSingleThreadScheduledExecutor()
	{
		return newScheduledThreadPool(1);
	}

	jxx::Ptr<jxx::lang::Object> Executors::cloneImpl() const
	{
		throw jxx::lang::CloneNotSupportedException();
	}
}
