#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
class CountTask final : public ::jxx::lang::Runnable {
public:
    explicit CountTask(std::atomic<int>& count) : count_(count) {}
    void run() override { count_.fetch_add(1, std::memory_order_relaxed); }
private:
    std::atomic<int>& count_;
};

TEST(ScheduledExecutorPeriodicParityTest, FixedRateTaskRepeatsUntilCancelled) {
    std::atomic<int> count{0};
    const auto executor =
        ::jxx::util::concurrent::Executors::newSingleThreadScheduledExecutor();
    const auto future = executor->scheduleAtFixedRate(
        ::jxx::NEW<CountTask>(count), 0, 1,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_TRUE(future->cancel(false));
    EXPECT_GE(count.load(std::memory_order_relaxed), 1);
    executor->shutdownNow();
}

TEST(ScheduledExecutorPeriodicParityTest, FixedDelayTaskRepeatsUntilCancelled) {
    std::atomic<int> count{0};
    const auto executor =
        ::jxx::util::concurrent::Executors::newSingleThreadScheduledExecutor();
    const auto future = executor->scheduleWithFixedDelay(
        ::jxx::NEW<CountTask>(count), 0, 1,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    EXPECT_TRUE(future->cancel(false));
    EXPECT_GE(count.load(std::memory_order_relaxed), 1);
    executor->shutdownNow();
}

TEST(ScheduledExecutorPeriodicParityTest, NonPositivePeriodsAreRejected) {
    std::atomic<int> count{0};
    const auto executor =
        ::jxx::util::concurrent::Executors::newSingleThreadScheduledExecutor();
    const auto command = ::jxx::NEW<CountTask>(count);
    EXPECT_THROW(
        executor->scheduleAtFixedRate(
            command, 0, 0,
            ::jxx::util::concurrent::TimeUnit::MILLISECONDS()),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(
        executor->scheduleWithFixedDelay(
            command, 0, 0,
            ::jxx::util::concurrent::TimeUnit::MILLISECONDS()),
        ::jxx::lang::IllegalArgumentException);
    executor->shutdownNow();
}

} // namespace
