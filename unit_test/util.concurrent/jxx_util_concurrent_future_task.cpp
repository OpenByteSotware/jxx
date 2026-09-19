#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.Integer.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.TimeoutException.h"

namespace {
using I = jxx::lang::Integer;

class ValueCallable final : public jxx::util::concurrent::Callable<I> {
public:
    jxx::Ptr<I> call() override { return I::valueOf(42); }
};

TEST(FutureTaskTest, CallableResultIsPublishedOnce) {
    auto task = jxx::NEW<jxx::util::concurrent::FutureTask<I>>(
        jxx::NEW<ValueCallable>());
    EXPECT_FALSE(task->isDone());
    task->run();
    ASSERT_TRUE(task->isDone());
    EXPECT_EQ(task->get()->intValue(), 42);
    task->run();
    EXPECT_EQ(task->get()->intValue(), 42);
}

TEST(FutureTaskTest, CancellationBeforeRunPreventsExecution) {
    auto task = jxx::NEW<jxx::util::concurrent::FutureTask<I>>(
        jxx::NEW<ValueCallable>());
    EXPECT_TRUE(task->cancel(false));
    task->run();
    EXPECT_TRUE(task->isCancelled());
    EXPECT_THROW(task->get(), jxx::util::concurrent::CancellationException);
}

TEST(FutureTaskTest, TimedGetBeforeRunTimesOut) {
    auto task = jxx::NEW<jxx::util::concurrent::FutureTask<I>>(
        jxx::NEW<ValueCallable>());
    EXPECT_THROW(task->get(2, jxx::util::concurrent::TimeUnit::MILLISECONDS()),
                 jxx::util::concurrent::TimeoutException);
}
}
