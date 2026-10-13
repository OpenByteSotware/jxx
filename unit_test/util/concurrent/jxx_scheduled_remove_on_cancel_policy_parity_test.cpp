#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NoOp final : public ::jxx::lang::ClassBase<NoOp,
    ::jxx::lang::Object, ::jxx::lang::Runnable> {
public: void run() override {}
};

TEST(ScheduledRemoveOnCancelPolicyParity, DefaultsFalseAndRoundTrips) {
    using namespace ::jxx::util::concurrent;
    const auto executor = ::jxx::NEW<ScheduledThreadPoolExecutor>(1);
    EXPECT_FALSE(executor->getRemoveOnCancelPolicy());
    executor->setRemoveOnCancelPolicy(true);
    EXPECT_TRUE(executor->getRemoveOnCancelPolicy());
    executor->shutdownNow();
}

TEST(ScheduledRemoveOnCancelPolicyParity, CancelledDelayedTaskIsRemovedImmediately) {
    using namespace ::jxx::util::concurrent;
    const auto executor = ::jxx::NEW<ScheduledThreadPoolExecutor>(1);
    executor->setRemoveOnCancelPolicy(true);
    const auto future = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()),
        1, TimeUnit::DAYS());
    EXPECT_TRUE(future->cancel(false));
    const auto pending = executor->shutdownNow();
    ASSERT_NE(nullptr, pending);
    EXPECT_EQ(0, pending->size());
}
} // namespace
