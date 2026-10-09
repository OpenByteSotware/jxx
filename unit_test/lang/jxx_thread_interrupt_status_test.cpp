#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"

namespace {
using namespace std::chrono_literals;
using ::jxx::lang::Object;
using ::jxx::lang::Runnable;
using ::jxx::lang::Thread;

void waitUntilInterruptStatusTest(const std::atomic<bool>& value) {
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (!value.load() &&
           std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }
    ASSERT_TRUE(value.load());
}

class InterruptStatusRunnable final
    : public ::jxx::lang::ClassBase<
          InterruptStatusRunnable,
          Object,
          Runnable> {
public:
    InterruptStatusRunnable(
        std::atomic<bool>& started,
        std::atomic<bool>& stop)
        : started_(started),
          stop_(stop) {
    }

    void run() override {
        started_.store(true);

        while (!stop_.load()) {
            std::this_thread::yield();
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& stop_;
};

TEST(ThreadTest, InterruptStatusCanBeObserved) {
    std::atomic<bool> started{false};
    std::atomic<bool> stop{false};

    const auto target =
        ::jxx::NEW<InterruptStatusRunnable>(
            started,
            stop);

    const auto thread =
        ::jxx::NEW<Thread>(
            ::jxx::CAST<Runnable>(target));

    EXPECT_FALSE(thread->isInterrupted());

    thread->start();
    waitUntilInterruptStatusTest(started);

    thread->interrupt();

    EXPECT_TRUE(thread->isInterrupted());

    stop.store(true);
    thread->join();

    EXPECT_FALSE(thread->isAlive());
}

TEST(ThreadTest, InterruptBeforeStartHasNoEffect) {
    const auto thread = ::jxx::NEW<Thread>();

    ASSERT_EQ(Thread::State::NEW, thread->getState());

    thread->interrupt();

    EXPECT_FALSE(thread->isInterrupted());
}

} // namespace
