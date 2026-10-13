#include <gtest/gtest.h>
#include <limits>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NoOpPeriodic final
    : public ::jxx::lang::ClassBase<NoOpPeriodic,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledPeriodicModeParity, FixedDelayDoesNotUseNegativePeriodEncoding) {
    using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;
    const auto task = ::jxx::NEW<Task>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpPeriodic>()),
        nullptr,
        0,
        std::numeric_limits<::jxx::lang::jlong>::max(),
        Task::PeriodicMode::FixedDelay);
    task->run();
    EXPECT_TRUE(task->isPeriodic());
    EXPECT_FALSE(task->isDone());
    EXPECT_GT(task->getDelay(
        ::jxx::util::concurrent::TimeUnit::NANOSECONDS()), 0);
}

TEST(ScheduledPeriodicModeParity, FixedRateRemainsPeriodic) {
    using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;
    const auto task = ::jxx::NEW<Task>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpPeriodic>()),
        nullptr, 0, 1, Task::PeriodicMode::FixedRate);
    task->run();
    EXPECT_TRUE(task->isPeriodic());
    EXPECT_FALSE(task->isDone());
}

TEST(ScheduledPeriodicModeParity, PeriodicModeRejectsNonPositiveMagnitude) {
    using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;
    EXPECT_ANY_THROW(::jxx::NEW<Task>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpPeriodic>()),
        nullptr, 0, 0, Task::PeriodicMode::FixedDelay));
}
} // namespace
