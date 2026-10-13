#include <gtest/gtest.h>
#include <limits>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NoOp final
    : public ::jxx::lang::ClassBase<
          NoOp,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledFutureRegression, SubmitFutureIsAlsoScheduledFuture) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto future = executor->submit(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()));
    EXPECT_NE(nullptr,
        ::jxx::CAST<ScheduledFuture<::jxx::lang::Object>>(future));
    executor->shutdown();
}

TEST(ScheduledFutureRegression, LegacyPositivePeriodConstructorIsFixedRate) {
    using Task = ::jxx::util::concurrent::ScheduledFutureTask<
        ::jxx::lang::Object>;
    const auto task = ::jxx::NEW<Task>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()),
        nullptr,
        0,
        std::numeric_limits<::jxx::lang::jlong>::max());
    task->run();
    EXPECT_TRUE(task->isPeriodic());
    EXPECT_FALSE(task->isDone());
    EXPECT_GT(task->getDelay(
        ::jxx::util::concurrent::TimeUnit::NANOSECONDS()), 0);
}
} // namespace
