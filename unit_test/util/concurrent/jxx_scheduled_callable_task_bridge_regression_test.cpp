#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledTask.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class BridgeCallable final
    : public ::jxx::lang::ClassBase<BridgeCallable,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::Callable<::jxx::lang::Object>> {
public:
    explicit BridgeCallable(const ::jxx::Ptr<::jxx::lang::Object>& value)
        : value_(value) {}

    ::jxx::Ptr<::jxx::lang::Object> call() override { return value_; }

private:
    ::jxx::Ptr<::jxx::lang::Object> value_;
};

TEST(ScheduledCallableTaskBridgeRegression, TypedTaskImplementsErasedQueueInterface) {
    using namespace ::jxx::util::concurrent;
    const auto callable = ::jxx::CAST<Callable<::jxx::lang::Object>>(
        ::jxx::NEW<BridgeCallable>(::jxx::NEW<::jxx::lang::Object>()));
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        callable, 0);
    EXPECT_NE(nullptr,
        ::jxx::CAST<scheduled_detail::ScheduledTask>(task));
}

TEST(ScheduledCallableTaskBridgeRegression, CallableRunsAndReturnsValue) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto value = ::jxx::NEW<::jxx::lang::Object>();
    const auto callable = ::jxx::CAST<Callable<::jxx::lang::Object>>(
        ::jxx::NEW<BridgeCallable>(value));
    const auto future = executor->schedule<::jxx::lang::Object>(
        callable, 1, TimeUnit::MILLISECONDS());
    ASSERT_NE(nullptr, future);
    EXPECT_EQ(value, future->get());
    executor->shutdown();
}

} // namespace
