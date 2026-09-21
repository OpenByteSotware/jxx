#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.ThreadGroup.h"

namespace {
using namespace std::chrono_literals;
using jxx::lang::Object;
using jxx::lang::Runnable;
using jxx::lang::String;
using jxx::lang::Thread;
using jxx::lang::ThreadGroup;

constexpr auto readinessTimeout = 5s;
constexpr jxx::lang::jlong cleanupJoinMillis = 5000;

bool waitUntil(
    const std::function<bool()>& condition,
    std::chrono::steady_clock::duration timeout = readinessTimeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (!condition()) {
        if (std::chrono::steady_clock::now() >= deadline) {
            return false;
        }
        std::this_thread::yield();
    }
    return true;
}

void interruptAndJoin(
    const jxx::Ptr<ThreadGroup>& group,
    const jxx::Ptr<Thread>& thread) {
    if (group != nullptr) {
        group->interrupt();
    }
    if (thread != nullptr && thread->isAlive()) {
        thread->interrupt();
        thread->join(cleanupJoinMillis);
    }
}

class BlockingRunnable final
    : public jxx::lang::ClassBase<BlockingRunnable, Object, Runnable> {
public:
    BlockingRunnable(
        std::atomic<bool>& started,
        std::atomic<bool>& interrupted)
        : started_(started), interrupted_(interrupted) {}

    void run() override {
        started_.store(true, std::memory_order_release);
        try {
            Thread::sleep(30000);
        }
        catch (const jxx::lang::InterruptedException&) {
            interrupted_.store(true, std::memory_order_release);
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& interrupted_;
};

class ControlledRaceRunnable final
    : public jxx::lang::ClassBase<ControlledRaceRunnable, Object, Runnable> {
public:
    ControlledRaceRunnable(
        std::atomic<bool>& started,
        std::atomic<bool>& release,
        std::atomic<bool>& observedInterrupt)
        : started_(started),
          release_(release),
          observedInterrupt_(observedInterrupt) {}

    void run() override {
        started_.store(true, std::memory_order_release);
        while (!release_.load(std::memory_order_acquire)) {
            if (Thread::interrupted()) {
                observedInterrupt_.store(true, std::memory_order_release);
                return;
            }
            Thread::yield();
        }
        if (Thread::interrupted()) {
            observedInterrupt_.store(true, std::memory_order_release);
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& release_;
    std::atomic<bool>& observedInterrupt_;
};

TEST(ThreadGroupInterruptionRaceTest,
     InterruptRacingWithNaturalCompletionLeavesConsistentTerminalState) {
    for (int iteration = 0; iteration < 32; ++iteration) {
        auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("race"));
        std::atomic<bool> started{false};
        std::atomic<bool> release{false};
        std::atomic<bool> observedInterrupt{false};
        auto thread = jxx::NEW<Thread>(
            group,
            jxx::CAST<Runnable>(jxx::NEW<ControlledRaceRunnable>(
                started,
                release,
                observedInterrupt)),
            jxx::NEW<String>("race-member"));

        thread->start();
        if (!waitUntil([&] {
                return started.load(std::memory_order_acquire);
            })) {
            release.store(true, std::memory_order_release);
            interruptAndJoin(group, thread);
            FAIL() << "Worker did not start before the readiness deadline";
        }

        std::thread interrupter([&] { group->interrupt(); });
        release.store(true, std::memory_order_release);
        interrupter.join();
        thread->join(cleanupJoinMillis);

        if (thread->isAlive()) {
            interruptAndJoin(group, thread);
        }

        ASSERT_FALSE(thread->isAlive());
        EXPECT_EQ(thread->getState(), Thread::State::TERMINATED);
        EXPECT_EQ(group->activeCount(), 0);
        (void)observedInterrupt;
    }
}

TEST(ThreadGroupInterruptionRaceTest,
     ConcurrentRepeatedInterruptsLeaveMemberAndGroupConsistent) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("repeat-race"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<BlockingRunnable>(started, interrupted)),
        jxx::NEW<String>("repeat-race-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    std::thread first([&] { group->interrupt(); });
    std::thread second([&] { group->interrupt(); });
    first.join();
    second.join();
    thread->join(cleanupJoinMillis);

    if (thread->isAlive()) {
        interruptAndJoin(group, thread);
    }

    ASSERT_FALSE(thread->isAlive());
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupTimeoutTest,
     TimedJoinReturnsWhileGroupMemberRemainsActive) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("join-timeout"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<BlockingRunnable>(started, interrupted)),
        jxx::NEW<String>("timed-join-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    thread->join(5);
    const bool remainedAlive = thread->isAlive();
    const auto activeAfterTimeout = group->activeCount();

    interruptAndJoin(group, thread);

    ASSERT_TRUE(remainedAlive);
    EXPECT_EQ(activeAfterTimeout, 1);
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_FALSE(thread->isAlive());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupTimeoutTest,
     ZeroTimeoutJoinWaitsUntilGroupInterruptTerminatesMember) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("zero-timeout"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    std::atomic<bool> joinStarted{false};
    std::atomic<bool> joinReturned{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<BlockingRunnable>(started, interrupted)),
        jxx::NEW<String>("zero-timeout-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    std::thread joiner([&] {
        joinStarted.store(true, std::memory_order_release);
        thread->join(0);
        joinReturned.store(true, std::memory_order_release);
    });

    if (!waitUntil([&] { return joinStarted.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        joiner.join();
        FAIL() << "Joiner did not start before the readiness deadline";
    }

    EXPECT_FALSE(joinReturned.load(std::memory_order_acquire));
    group->interrupt();

    if (!waitUntil(
            [&] { return joinReturned.load(std::memory_order_acquire); },
            readinessTimeout)) {
        interruptAndJoin(group, thread);
    }

    joiner.join();
    if (thread->isAlive()) {
        interruptAndJoin(group, thread);
    }

    ASSERT_TRUE(joinReturned.load(std::memory_order_acquire));
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_FALSE(thread->isAlive());
    EXPECT_EQ(group->activeCount(), 0);
}
} // namespace
