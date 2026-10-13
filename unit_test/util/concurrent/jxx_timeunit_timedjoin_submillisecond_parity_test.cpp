#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class SlowRunnable final
    : public ::jxx::lang::ClassBase<
          SlowRunnable,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {
        ::jxx::lang::Thread::sleep(100);
    }
};

TEST(TimeUnitTimedJoinParity, PositiveSubMillisecondTimeoutIsNotIndefinite) {
    using namespace ::jxx::util::concurrent;
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<SlowRunnable>()));
    thread->start();

    TimeUnit::NANOSECONDS()->timedJoin(thread, 1);
    EXPECT_TRUE(thread->isAlive());

    thread->join();
}

TEST(TimeUnitTimedJoinParity, NullThreadIsRejectedEvenForZeroTimeout) {
    using namespace ::jxx::util::concurrent;
    EXPECT_ANY_THROW(TimeUnit::NANOSECONDS()->timedJoin(nullptr, 0));
}

} // namespace
