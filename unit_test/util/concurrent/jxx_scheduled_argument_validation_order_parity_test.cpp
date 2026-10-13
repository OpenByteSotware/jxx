#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NoOp final
    : public ::jxx::lang::ClassBase<NoOp,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledArgumentValidationParity,
     NullCommandPrecedesInvalidFixedRatePeriod) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newSingleThreadScheduledExecutor();
    EXPECT_THROW(service->scheduleAtFixedRate(
        nullptr, 0, 0, TimeUnit::NANOSECONDS()),
        ::jxx::lang::NullPointerException);
    service->shutdownNow();
}

TEST(ScheduledArgumentValidationParity,
     NullCommandPrecedesInvalidFixedDelay) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newSingleThreadScheduledExecutor();
    EXPECT_THROW(service->scheduleWithFixedDelay(
        nullptr, 0, 0, TimeUnit::NANOSECONDS()),
        ::jxx::lang::NullPointerException);
    service->shutdownNow();
}

TEST(ScheduledArgumentValidationParity,
     ValidCommandWithNonPositivePeriodIsIllegalArgument) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newSingleThreadScheduledExecutor();
    const auto command = ::jxx::CAST<::jxx::lang::Runnable>(
        ::jxx::NEW<NoOp>());
    EXPECT_THROW(service->scheduleAtFixedRate(
        command, 0, 0, TimeUnit::NANOSECONDS()),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(service->scheduleWithFixedDelay(
        command, 0, -1, TimeUnit::NANOSECONDS()),
        ::jxx::lang::IllegalArgumentException);
    service->shutdownNow();
}
} // namespace
