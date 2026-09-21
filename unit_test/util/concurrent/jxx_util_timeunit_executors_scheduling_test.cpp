#include <gtest/gtest.h>

#include <atomic>
#include <limits>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledFuture.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
using ::jxx::util::concurrent::Executors;
using ::jxx::util::concurrent::TimeUnit;

class Increment final : public ::jxx::lang::Runnable {
public:
    explicit Increment(std::atomic<int>& value) : value_(value) {}
    void run() override { value_.fetch_add(1, std::memory_order_relaxed); }
private:
    std::atomic<int>& value_;
};

TEST(TimeUnitParityTest, ConvertsAndSaturates) {
    EXPECT_EQ(1000, TimeUnit::SECONDS()->toMillis(1));
    EXPECT_EQ(60, TimeUnit::HOURS()->toMinutes(1));
    EXPECT_EQ(0, TimeUnit::NANOSECONDS()->toMillis(999999));
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jlong>::max(),
              TimeUnit::DAYS()->toNanos(
                  std::numeric_limits<::jxx::lang::jlong>::max()));
}

TEST(TimeUnitParityTest, NonPositiveSleepAndJoinReturnImmediately) {
    TimeUnit::NANOSECONDS()->sleep(0);
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>();
    TimeUnit::SECONDS()->timedJoin(thread, 0);
}

TEST(ExecutorsSchedulingParityTest, FactoryCreatesOneShotScheduler) {
    std::atomic<int> count{0};
    const auto scheduler = Executors::newSingleThreadScheduledExecutor();
    const auto future = scheduler->schedule(
        ::jxx::NEW<Increment>(count), 0, TimeUnit::MILLISECONDS());
    ASSERT_NE(nullptr, future);
    (void)future->get();
    EXPECT_EQ(1, count.load(std::memory_order_relaxed));
    scheduler->shutdown();
}

TEST(ExecutorsSchedulingParityTest, InvalidPoolSizesAreRejected) {
    EXPECT_THROW(Executors::newFixedThreadPool(0),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(Executors::newScheduledThreadPool(0),
                 ::jxx::lang::IllegalArgumentException);
}

} // namespace
