#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class MarkRun final
    : public ::jxx::lang::ClassBase<MarkRun,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override { ran_.store(true); }
    ::jxx::lang::jbool ran() const noexcept { return ran_.load(); }
private:
    std::atomic<::jxx::lang::jbool> ran_{false};
};

TEST(ScheduledExecutorZeroCoreParity, FactoryAcceptsZeroCorePoolSize) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newScheduledThreadPool(0);
    ASSERT_NE(nullptr, service);
    const auto concrete = ::jxx::CAST<ScheduledThreadPoolExecutor>(service);
    ASSERT_NE(nullptr, concrete);
    EXPECT_EQ(0, concrete->getCorePoolSize());
    service->shutdownNow();
}

TEST(ScheduledExecutorZeroCoreParity, ZeroCorePoolCanExecuteEligibleTask) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newScheduledThreadPool(0);
    const auto runnable = ::jxx::NEW<MarkRun>();
    const auto future = service->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(runnable),
        0,
        TimeUnit::NANOSECONDS());
    (void)future->get(2, TimeUnit::SECONDS());
    EXPECT_TRUE(runnable->ran());
    service->shutdown();
}

TEST(ScheduledExecutorZeroCoreParity, NegativeCorePoolSizeIsRejected) {
    using namespace ::jxx::util::concurrent;
    EXPECT_ANY_THROW(Executors::newScheduledThreadPool(-1));
}
} // namespace
