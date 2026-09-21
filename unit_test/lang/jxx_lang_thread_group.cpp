#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <functional>
#include <string>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.ThreadGroup.h"
#include "lang/jxx.lang.buildin_array.h"

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
        std::this_thread::sleep_for(1ms);
    }
    return true;
}

bool interruptAndJoin(
    const jxx::Ptr<ThreadGroup>& group,
    const jxx::Ptr<Thread>& thread) {
    if (group != nullptr) {
        group->interrupt();
    }
    if (thread == nullptr || !thread->isAlive()) {
        return true;
    }
    thread->interrupt();
    thread->join(cleanupJoinMillis);
    return !thread->isAlive();
}

class GroupSleepingRunnable final
    : public jxx::lang::ClassBase<GroupSleepingRunnable, Object, Runnable> {
public:
    GroupSleepingRunnable(
        std::atomic<bool>& started,
        std::atomic<bool>& interrupted,
        std::atomic<bool>* statusCleared = nullptr)
        : started_(started),
          interrupted_(interrupted),
          statusCleared_(statusCleared) {}

    void run() override {
        started_.store(true, std::memory_order_release);
        try {
            Thread::sleep(30000);
        }
        catch (const jxx::lang::InterruptedException&) {
            interrupted_.store(true, std::memory_order_release);
            if (statusCleared_ != nullptr) {
                const auto current = Thread::currentThread();
                statusCleared_->store(
                    current != nullptr && !current->isInterrupted(),
                    std::memory_order_release);
            }
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& interrupted_;
    std::atomic<bool>* statusCleared_;
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

TEST(ThreadGroupTest, ParentHierarchyAndNamesArePreserved) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("parent"));
    auto child = jxx::NEW<ThreadGroup>(parent, jxx::NEW<String>("child"));

    EXPECT_EQ(parent->getName()->utf8(), "parent");
    EXPECT_EQ(child->getName()->utf8(), "child");
    EXPECT_EQ(child->getParent().get(), parent.get());
    EXPECT_TRUE(parent->parentOf(child));
    EXPECT_TRUE(parent->parentOf(parent));
    EXPECT_FALSE(child->parentOf(parent));
    EXPECT_GE(parent->activeGroupCount(), 1);
}

TEST(ThreadGroupTest, GroupPriorityCapsMemberThreadPriority) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("limited"));
    group->setMaxPriority(3);
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::Ptr<Runnable>{},
        jxx::NEW<String>("member"));

    thread->setPriority(Thread::MAX_PRIORITY);

    EXPECT_EQ(group->getMaxPriority(), 3);
    EXPECT_EQ(thread->getPriority(), 3);
}

TEST(ThreadGroupTest, ActiveThreadsCanBeEnumerated) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("workers"));
    std::atomic<bool> firstStarted{false};
    std::atomic<bool> firstInterrupted{false};
    std::atomic<bool> secondStarted{false};
    std::atomic<bool> secondInterrupted{false};

    auto first = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            firstStarted, firstInterrupted)),
        jxx::NEW<String>("first"));
    auto second = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            secondStarted, secondInterrupted)),
        jxx::NEW<String>("second"));

    first->start();
    second->start();
    if (!waitUntil([&] {
            return firstStarted.load(std::memory_order_acquire) &&
                   secondStarted.load(std::memory_order_acquire);
        })) {
        interruptAndJoin(group, first);
        interruptAndJoin(group, second);
        FAIL() << "Workers did not start before the readiness deadline";
    }

    auto threads =
        jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<Thread>, 1>>(4);
    const auto count = group->enumerate(threads, false);

    EXPECT_EQ(group->activeCount(), 2);
    EXPECT_EQ(count, 2);
    ASSERT_NE((*threads)[0], nullptr);
    ASSERT_NE((*threads)[1], nullptr);

    const bool firstStopped = interruptAndJoin(group, first);
    const bool secondStopped = interruptAndJoin(group, second);
    ASSERT_TRUE(firstStopped);
    ASSERT_TRUE(secondStopped);
    EXPECT_TRUE(firstInterrupted.load(std::memory_order_acquire));
    EXPECT_TRUE(secondInterrupted.load(std::memory_order_acquire));
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupTest, DaemonAndStringRepresentationAreObservable) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("diagnostic"));
    group->setDaemon(true);

    EXPECT_TRUE(group->isDaemon());
    EXPECT_NE(
        group->toString()->utf8().find("diagnostic"),
        std::string::npos);
}

TEST(ThreadGroupInterruptionTest, GroupInterruptPropagatesToMembers) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("interruptible"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    const bool stopped = interruptAndJoin(group, thread);
    ASSERT_TRUE(stopped);
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, ParentInterruptReachesChildGroupThreads) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("parent-interrupt"));
    auto child = jxx::NEW<ThreadGroup>(
        parent, jxx::NEW<String>("child-interrupt"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        child,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("child-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(parent, thread);
        FAIL() << "Child-group worker did not start";
    }

    const bool stopped = interruptAndJoin(parent, thread);
    ASSERT_TRUE(stopped);
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_EQ(parent->activeCount(), 0);
    EXPECT_EQ(child->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, InterruptedExceptionClearsMemberStatus) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("clear-status"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    std::atomic<bool> statusCleared{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted, &statusCleared)),
        jxx::NEW<String>("clear-status-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    const bool stopped = interruptAndJoin(group, thread);
    ASSERT_TRUE(stopped);
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_TRUE(statusCleared.load(std::memory_order_acquire));
}

TEST(ThreadGroupInterruptionTest, InterruptDoesNotMarkUnstartedMember) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("unstarted"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("not-started"));

    group->interrupt();

    EXPECT_FALSE(thread->isInterrupted());
    EXPECT_EQ(thread->getState(), Thread::State::NEW);
}

TEST(ThreadGroupInterruptionTest, DaemonGroupInterruptsActiveMember) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("daemon-interrupt"));
    group->setDaemon(true);
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("daemon-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Daemon-group worker did not start";
    }

    const bool stopped = interruptAndJoin(group, thread);
    ASSERT_TRUE(stopped);
    EXPECT_TRUE(group->isDaemon());
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, EmptyDaemonGroupInterruptIsSafe) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("daemon-parent"));
    auto group = jxx::NEW<ThreadGroup>(
        parent, jxx::NEW<String>("empty-daemon"));
    group->setDaemon(true);

    EXPECT_NO_THROW(group->interrupt());
    EXPECT_TRUE(group->isDaemon());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, TerminatedMembersAreIgnored) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("terminated-members"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("terminated-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }
    ASSERT_TRUE(interruptAndJoin(group, thread));
    ASSERT_EQ(group->activeCount(), 0);

    EXPECT_NO_THROW(group->interrupt());
    EXPECT_FALSE(thread->isInterrupted());
    EXPECT_EQ(thread->getState(), Thread::State::TERMINATED);
}

TEST(ThreadGroupInterruptionTest, DestroyedEmptyGroupInterruptIsSafe) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("destroy-parent"));
    auto group = jxx::NEW<ThreadGroup>(
        parent, jxx::NEW<String>("destroyed"));
    group->destroy();

    ASSERT_TRUE(group->isDestroyed());
    EXPECT_NO_THROW(group->interrupt());
    EXPECT_EQ(group->activeCount(), 0);
}

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
                started, release, observedInterrupt)),
            jxx::NEW<String>("race-member"));

        thread->start();
        if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
            release.store(true, std::memory_order_release);
            interruptAndJoin(group, thread);
            FAIL() << "Race worker did not start";
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
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
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

TEST(ThreadGroupTimeoutTest, TimedJoinReturnsWhileGroupMemberRemainsActive) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("join-timeout"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(jxx::NEW<GroupSleepingRunnable>(
            started, interrupted)),
        jxx::NEW<String>("timed-join-member"));

    thread->start();
    if (!waitUntil([&] { return started.load(std::memory_order_acquire); })) {
        interruptAndJoin(group, thread);
        FAIL() << "Worker did not start before the readiness deadline";
    }

    thread->join(5);
    const bool remainedAlive = thread->isAlive();
    const auto activeAfterTimeout = group->activeCount();
    const bool stopped = interruptAndJoin(group, thread);

    ASSERT_TRUE(stopped);
    EXPECT_TRUE(remainedAlive);
    EXPECT_GE(activeAfterTimeout, 1);
    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_EQ(group->activeCount(), 0);
}
} // namespace
