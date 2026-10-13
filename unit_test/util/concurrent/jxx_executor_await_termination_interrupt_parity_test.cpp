#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NoOp final
    : public ::jxx::lang::ClassBase<NoOp,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {}
};

class TerminationWaiter final
    : public ::jxx::lang::ClassBase<TerminationWaiter,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    explicit TerminationWaiter(
        const ::jxx::Ptr<::jxx::util::concurrent::ScheduledExecutorService>& service)
        : service_(service) {}

    void run() override {
        try {
            (void)service_->awaitTermination(
                30, ::jxx::util::concurrent::TimeUnit::SECONDS());
        } catch (const ::jxx::lang::InterruptedException&) {
            interrupted_.store(true);
        }
    }

    ::jxx::lang::jbool interrupted() const noexcept {
        return interrupted_.load();
    }

private:
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledExecutorService> service_;
    std::atomic<::jxx::lang::jbool> interrupted_{false};
};

TEST(ExecutorAwaitTerminationParity, ScheduledExecutorWaitIsInterruptible) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newScheduledThreadPool(1);
    (void)service->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()),
        1, TimeUnit::DAYS());
    service->shutdown();

    const auto waiter = ::jxx::NEW<TerminationWaiter>(service);
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(waiter));
    thread->start();
    ::jxx::lang::Thread::sleep(20);
    thread->interrupt();
    thread->join(2000);

    EXPECT_TRUE(waiter->interrupted());
    EXPECT_FALSE(thread->isAlive());
    (void)service->shutdownNow();
}

TEST(ExecutorAwaitTerminationParity, NonPositiveTimeoutReturnsImmediately) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newScheduledThreadPool(1);
    EXPECT_FALSE(service->awaitTermination(0, TimeUnit::NANOSECONDS()));
    service->shutdownNow();
}
} // namespace
