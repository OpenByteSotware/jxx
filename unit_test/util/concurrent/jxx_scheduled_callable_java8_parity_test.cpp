#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class ObjectCallable final
    : public ::jxx::lang::ClassBase<
          ObjectCallable,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::Callable<::jxx::lang::Object>> {
public:
    explicit ObjectCallable(const ::jxx::Ptr<::jxx::lang::Object>& value)
        : value_(value) {}

    ::jxx::Ptr<::jxx::lang::Object> call() override {
        called_ = true;
        return value_;
    }

    ::jxx::lang::jbool called() const noexcept { return called_; }

private:
    ::jxx::Ptr<::jxx::lang::Object> value_;
    ::jxx::lang::jbool called_ = false;
};

TEST(ScheduledExecutorServiceJava8Parity, CallableFutureTracksDelayAndResult) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto value = ::jxx::NEW<::jxx::lang::Object>();
    const auto callable = ::jxx::NEW<ObjectCallable>(value);

    const auto future = executor->schedule<::jxx::lang::Object>(
        ::jxx::CAST<Callable<::jxx::lang::Object>>(callable),
        20,
        TimeUnit::MILLISECONDS());

    ASSERT_NE(nullptr, future);
    EXPECT_GE(future->getDelay(TimeUnit::MILLISECONDS()), 0);
    EXPECT_EQ(value, future->get());
    EXPECT_TRUE(callable->called());
    executor->shutdown();
}

TEST(ScheduledExecutorServiceJava8Parity, CancellingCallablePreventsInvocation) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto callable = ::jxx::NEW<ObjectCallable>(
        ::jxx::NEW<::jxx::lang::Object>());

    const auto future = executor->schedule<::jxx::lang::Object>(
        ::jxx::CAST<Callable<::jxx::lang::Object>>(callable),
        1,
        TimeUnit::DAYS());

    EXPECT_TRUE(future->cancel(false));
    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());
    EXPECT_FALSE(callable->called());
    executor->shutdownNow();
}

TEST(ScheduledExecutorServiceJava8Parity, RejectsNullCallableAndUnit) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newSingleThreadScheduledExecutor();
    const auto callable = ::jxx::CAST<Callable<::jxx::lang::Object>>(
        ::jxx::NEW<ObjectCallable>(::jxx::NEW<::jxx::lang::Object>()));

    EXPECT_ANY_THROW(executor->schedule<::jxx::lang::Object>(
        nullptr, 0, TimeUnit::SECONDS()));
    EXPECT_ANY_THROW(executor->schedule<::jxx::lang::Object>(
        callable, 0, nullptr));
    executor->shutdownNow();
}

} // namespace
