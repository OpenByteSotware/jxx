#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class ValueCallable final : public ::jxx::lang::ClassBase<ValueCallable, ::jxx::lang::Object, ::jxx::util::concurrent::Callable<::jxx::lang::Object>> {
public:
    explicit ValueCallable(const ::jxx::Ptr<::jxx::lang::Object>& value) : value_(value) {}
    ::jxx::Ptr<::jxx::lang::Object> call() override { return value_; }
private: ::jxx::Ptr<::jxx::lang::Object> value_;
};
class NoOp final : public ::jxx::lang::ClassBase<NoOp, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public: void run() override {}
};
TEST(ScheduledExecutorPriorityFixes, CallableUsesSingleQueuedFuture) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto callable = ::jxx::CAST<Callable<::jxx::lang::Object>>(::jxx::NEW<ValueCallable>(::jxx::NEW<::jxx::lang::Object>()));
    const auto future = executor->schedule<::jxx::lang::Object>(callable, 1, TimeUnit::DAYS());
    EXPECT_TRUE(future->cancel(false));
    executor->shutdownNow();
    EXPECT_TRUE(future->isCancelled());
}
TEST(ScheduledExecutorPriorityFixes, ShutdownNowReturnsNeverStartedScheduledTasks) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    executor->schedule(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()), 1, TimeUnit::DAYS());
    const auto pending = executor->shutdownNow();
    ASSERT_NE(nullptr, pending);
    EXPECT_EQ(1, pending->size());
}
TEST(ScheduledExecutorPriorityFixes, SubmitReturnsScheduledFuture) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto future = executor->submit(::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()));
    EXPECT_NE(nullptr, ::jxx::CAST<ScheduledFuture<::jxx::lang::Object>>(future));
    executor->shutdown();
}
} // namespace
