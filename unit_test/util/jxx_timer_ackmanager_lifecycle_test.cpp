#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "jxx.initialize.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"

namespace {

class CountingTask final : public ::jxx::util::TimerTask {
public:
    explicit CountingTask(std::atomic<int>& count) : count_(count) {}
    void run() override { ++count_; }
private:
    std::atomic<int>& count_;
};

class ThrowingTask final : public ::jxx::util::TimerTask {
public:
    void run() override {
        throw ::jxx::lang::RuntimeException("task failure");
    }
};

bool waitForAtLeast(std::atomic<int>& value, int expected)
{
    for (int attempt = 0; attempt < 200; ++attempt) {
        if (value.load() >= expected) return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return value.load() >= expected;
}

TEST(TimerAckManagerParity, CancelledRetryTaskStopsRepeating)
{
    ::jxx::initialize();
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    auto task = ::jxx::NEW<CountingTask>(count);
    timer->schedule(task, 0, 5);
    ASSERT_TRUE(waitForAtLeast(count, 2));
    EXPECT_TRUE(task->cancel());
    const auto afterCancel = count.load();
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_EQ(count.load(), afterCancel);
    timer->cancel();
}

TEST(TimerAckManagerParity, CancelPreventsLaterScheduling)
{
    ::jxx::initialize();
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    timer->cancel();
    EXPECT_THROW(timer->schedule(::jxx::NEW<CountingTask>(count), 0),
        ::jxx::lang::IllegalStateException);
}

TEST(TimerAckManagerParity, UncheckedTaskFailureTerminatesTimerThread)
{
    ::jxx::initialize();
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    timer->schedule(::jxx::NEW<ThrowingTask>(), 0);
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    EXPECT_THROW(timer->schedule(::jxx::NEW<CountingTask>(count), 0),
        ::jxx::lang::IllegalStateException);
}

} // namespace
