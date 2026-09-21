#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.ExecutionException.h"
#include "util/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace {
class NoOp final : public ::jxx::lang::Runnable {
public: void run() override {}
};
class Fail final : public ::jxx::lang::Runnable {
public: void run() override { throw ::jxx::lang::RuntimeException(); }
};
using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;

TEST(ScheduledFutureCompletionParityTest, SuccessfulRunCompletesAndCannotBeCancelled) {
    const auto task = ::jxx::NEW<Task>(::jxx::NEW<NoOp>(), nullptr, 0);
    EXPECT_FALSE(task->isDone());
    task->run();
    EXPECT_TRUE(task->isDone());
    EXPECT_FALSE(task->isCancelled());
    EXPECT_EQ(nullptr, task->get());
    EXPECT_FALSE(task->cancel(false));
}

TEST(ScheduledFutureCompletionParityTest, CancellationCompletesAndGetThrows) {
    const auto task = ::jxx::NEW<Task>(::jxx::NEW<NoOp>(), nullptr, 1000000000LL);
    EXPECT_TRUE(task->cancel(false));
    EXPECT_TRUE(task->isDone());
    EXPECT_TRUE(task->isCancelled());
    EXPECT_FALSE(task->cancel(true));
    EXPECT_THROW(task->get(), ::jxx::util::concurrent::CancellationException);
}

TEST(ScheduledFutureCompletionParityTest, FailureCompletesExceptionally) {
    const auto task = ::jxx::NEW<Task>(::jxx::NEW<Fail>(), nullptr, 0);
    task->run();
    EXPECT_TRUE(task->isDone());
    EXPECT_THROW(task->get(), ::jxx::util::concurrent::ExecutionException);
}

TEST(ScheduledFutureCompletionParityTest, TimedGetTimesOutBeforeCompletion) {
    const auto task = ::jxx::NEW<Task>(::jxx::NEW<NoOp>(), nullptr, 1000000000LL);
    EXPECT_THROW(
        task->get(0, ::jxx::util::concurrent::TimeUnit::NANOSECONDS()),
        ::jxx::util::concurrent::TimeoutException);
}
}
