#include <gtest/gtest.h>
#include <limits>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class NoOpScheduledRunnable final
    : public ::jxx::lang::ClassBase<
          NoOpScheduledRunnable,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledFutureJava8Parity, ExtremePositiveDelayDoesNotWrapToExpired) {
    using namespace ::jxx::util::concurrent;
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<NoOpScheduledRunnable>()),
        nullptr,
        std::numeric_limits<::jxx::lang::jlong>::max());

    EXPECT_GT(task->getDelay(TimeUnit::NANOSECONDS()), 0);
    EXPECT_FALSE(task->isDone());
    EXPECT_FALSE(task->isCancelled());
}

TEST(ScheduledFutureJava8Parity, ExtremeFixedRateRescheduleRemainsFuture) {
    using namespace ::jxx::util::concurrent;
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<NoOpScheduledRunnable>()),
        nullptr,
        0,
        std::numeric_limits<::jxx::lang::jlong>::max());

    task->run();
    EXPECT_TRUE(task->isPeriodic());
    EXPECT_FALSE(task->isDone());
    EXPECT_GT(task->getDelay(TimeUnit::NANOSECONDS()), 0);
}

} // namespace
