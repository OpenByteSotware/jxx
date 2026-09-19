#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.CountDownLatch.h"

namespace {
class SleepUntilInterrupted final : public jxx::lang::Runnable {
public:
    SleepUntilInterrupted(std::atomic<bool>& started, std::atomic<bool>& caught)
        : started_(started), caught_(caught) {}
    void run() override {
        started_.store(true);
        try {
            jxx::lang::Thread::sleep(30000);
        } catch (const jxx::lang::InterruptedException&) {
            caught_.store(true);
        }
    }
private:
    std::atomic<bool>& started_;
    std::atomic<bool>& caught_;
};

class AwaitUntilInterrupted final : public jxx::lang::Runnable {
public:
    AwaitUntilInterrupted(const jxx::Ptr<jxx::util::concurrent::CountDownLatch>& latch,
                          std::atomic<bool>& started,
                          std::atomic<bool>& caught)
        : latch_(latch), started_(started), caught_(caught) {}
    void run() override {
        started_.store(true);
        try {
            latch_->await();
        } catch (const jxx::lang::InterruptedException&) {
            caught_.store(true);
        }
    }
private:
    jxx::Ptr<jxx::util::concurrent::CountDownLatch> latch_;
    std::atomic<bool>& started_;
    std::atomic<bool>& caught_;
};

void waitUntil(const std::atomic<bool>& value) {
    for (int i = 0; i < 1000 && !value.load(); ++i) {
        jxx::lang::Thread::yield();
    }
    ASSERT_TRUE(value.load());
}

TEST(InterruptionTest, SleepRespondsToInterrupt) {
    std::atomic<bool> started{false};
    std::atomic<bool> caught{false};
    auto thread = jxx::NEW<jxx::lang::Thread>(
        jxx::NEW<SleepUntilInterrupted>(started, caught));
    thread->start();
    waitUntil(started);
    thread->interrupt();
    thread->join(5000);
    EXPECT_TRUE(caught.load());
    EXPECT_FALSE(thread->isAlive());
}

TEST(InterruptionTest, LatchAwaitRespondsToInterrupt) {
    auto latch = jxx::NEW<jxx::util::concurrent::CountDownLatch>(1);
    std::atomic<bool> started{false};
    std::atomic<bool> caught{false};
    auto thread = jxx::NEW<jxx::lang::Thread>(
        jxx::NEW<AwaitUntilInterrupted>(latch, started, caught));
    thread->start();
    waitUntil(started);
    thread->interrupt();
    thread->join(5000);
    if (thread->isAlive()) latch->countDown();
    EXPECT_TRUE(caught.load());
    EXPECT_FALSE(thread->isAlive());
}

TEST(InterruptionTest, InterruptedClearsCurrentThreadFlag) {
    auto current = jxx::lang::Thread::currentThread();
    current->interrupt();
    EXPECT_TRUE(jxx::lang::Thread::interrupted());
    EXPECT_FALSE(jxx::lang::Thread::interrupted());
}
}
