#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class SleepingRunnable final
    : public ::jxx::lang::ClassBase<SleepingRunnable,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {
        try {
            ::jxx::util::concurrent::TimeUnit::SECONDS()->sleep(30);
        } catch (const ::jxx::lang::InterruptedException&) {
            interrupted_.store(true);
        }
    }
    ::jxx::lang::jbool interrupted() const noexcept {
        return interrupted_.load();
    }
private:
    std::atomic<::jxx::lang::jbool> interrupted_{false};
};

TEST(TimeUnitSleepParity, SleepRespondsToThreadInterrupt) {
    const auto runnable = ::jxx::NEW<SleepingRunnable>();
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(runnable));
    thread->start();
    ::jxx::lang::Thread::sleep(20);
    thread->interrupt();
    thread->join(2000);
    EXPECT_TRUE(runnable->interrupted());
    EXPECT_FALSE(thread->isAlive());
}

TEST(TimeUnitSleepParity, PositiveNanosecondTimeoutUsesNanosOverload) {
    EXPECT_NO_THROW(
        ::jxx::util::concurrent::TimeUnit::NANOSECONDS()->sleep(1));
}
} // namespace
