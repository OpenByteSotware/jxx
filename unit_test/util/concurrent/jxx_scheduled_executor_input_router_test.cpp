#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "jxx_scheduled_executor_test_support.h"

TEST(JxxScheduledExecutorInputRouterTest, RejectsInvalidPeriod) {
    const auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4);
    const auto runnable = jxxScheduledTestRunnable([] {});

    EXPECT_THROW(
        executor->scheduleAtFixedRate(
            runnable,
            0,
            0,
            ::jxx::util::concurrent::TimeUnit::MILLISECONDS()),
        ::jxx::lang::IllegalArgumentException);

    executor->shutdownNow();
}

TEST(JxxScheduledExecutorInputRouterTest, PeriodicFailureStopsLaterRuns) {
    const auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4);
    std::atomic<int> runCount{0};

    const auto future = executor->scheduleAtFixedRate(
        jxxScheduledTestRunnable([&] {
            ++runCount;
            throw ::jxx::lang::IllegalArgumentException();
        }),
        0,
        5,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    EXPECT_THROW(
        future->get(),
        ::jxx::util::concurrent::ExecutionException);

    EXPECT_TRUE(future->isDone());
    EXPECT_FALSE(future->isCancelled());
    EXPECT_EQ(1, runCount.load());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(1, runCount.load());

    executor->shutdownNow();
}

TEST(JxxScheduledExecutorInputRouterTest, RejectsAfterShutdown) {
    const auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4);
    executor->shutdown();

    EXPECT_THROW(
        executor->schedule(
            jxxScheduledTestRunnable([] {}),
            0,
            ::jxx::util::concurrent::TimeUnit::MILLISECONDS()),
        ::jxx::util::concurrent::RejectedExecutionException);
}

TEST(JxxScheduledExecutorInputRouterTest, PolicyChangeCancelsDelayedTask) {
    const auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4);
    const auto future = executor->schedule(
        jxxScheduledTestRunnable([] {}),
        1,
        ::jxx::util::concurrent::TimeUnit::HOURS());

    executor->shutdown();
    executor->setExecuteExistingDelayedTasksAfterShutdownPolicy(false);

    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());
}
