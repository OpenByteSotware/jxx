#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "jxx_scheduled_executor_test_support.h"

TEST(JxxScheduledExecutorPeriodicFailureTest, FailureCompletesFuture) {
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
        future->get(
            2,
            ::jxx::util::concurrent::TimeUnit::SECONDS()),
        ::jxx::util::concurrent::ExecutionException);

    EXPECT_TRUE(future->isDone());
    EXPECT_FALSE(future->isCancelled());
    EXPECT_EQ(1, runCount.load());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(1, runCount.load());

    executor->shutdownNow();
}
