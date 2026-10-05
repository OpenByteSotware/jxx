#include <gtest/gtest.h>
#include <atomic>
#include <chrono>
#include <thread>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "jxx_scheduled_executor_test_support.h"
TEST(JxxScheduledExecutorInputRouterTest, RejectsInvalidPeriod)
{
	auto e = ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4); auto r = jxxScheduledTestRunnable([]
   {}); EXPECT_THROW(e->scheduleAtFixedRate(r, 0, 0, ::jxx::util::concurrent::TimeUnit::MILLISECONDS()), ::jxx::lang::IllegalArgumentException); e->shutdownNow();
}
TEST(JxxScheduledExecutorInputRouterTest, PeriodicFailureStopsLaterRuns)
{
	auto e = ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4); std::atomic<int> n{ 0 }; auto f = e->scheduleAtFixedRate(jxxScheduledTestRunnable([&]
		{
			++n; throw ::jxx::lang::IllegalArgumentException();
		}), 0, 5, ::jxx::util::concurrent::TimeUnit::MILLISECONDS()); std::this_thread::sleep_for(std::chrono::milliseconds(100)); EXPECT_EQ(1, n.load()); EXPECT_TRUE(f->isDone()); e->shutdownNow();
}
TEST(JxxScheduledExecutorInputRouterTest, RejectsAfterShutdown)
{
	auto e = ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4); e->shutdown(); EXPECT_THROW(e->schedule(jxxScheduledTestRunnable([]
		{}), 0, ::jxx::util::concurrent::TimeUnit::MILLISECONDS()), ::jxx::util::concurrent::RejectedExecutionException);
}
TEST(JxxScheduledExecutorInputRouterTest, PolicyChangeCancelsDelayedTask)
{
	auto e = ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4); auto f = e->schedule(jxxScheduledTestRunnable([]
		{}), 1, ::jxx::util::concurrent::TimeUnit::HOURS()); e->shutdown(); e->setExecuteExistingDelayedTasksAfterShutdownPolicy(false); EXPECT_TRUE(f->isCancelled());
}
