#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
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

void waitUntil(const std::atomic<bool>& value) {
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (!value.load() && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }
    ASSERT_TRUE(value.load());
}

class GroupSleepingRunnable final
    : public jxx::lang::ClassBase<GroupSleepingRunnable, Object, Runnable> {
public:
    GroupSleepingRunnable(std::atomic<bool>& started,
                          std::atomic<bool>& interrupted,
                          std::atomic<bool>* statusCleared = nullptr)
        : started_(started),
          interrupted_(interrupted),
          statusCleared_(statusCleared) {}

    void run() override {
        started_.store(true);
        try {
            Thread::sleep(30000);
        }
        catch (const jxx::lang::InterruptedException&) {
            interrupted_.store(true);
            if (statusCleared_ != nullptr) {
                const auto current = Thread::currentThread();
                statusCleared_->store(
                    current != nullptr && !current->isInterrupted());
            }
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& interrupted_;
    std::atomic<bool>* statusCleared_;
};

TEST(ThreadGroupTest, ParentHierarchyAndNamesArePreserved) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("parent"));
    auto child = jxx::NEW<ThreadGroup>(
        parent,
        jxx::NEW<String>("child"));

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
    waitUntil(firstStarted);
    waitUntil(secondStarted);

    auto threads = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<Thread>, 1>>(4);
    const auto count = group->enumerate(threads, false);

    EXPECT_EQ(group->activeCount(), 2);
    EXPECT_EQ(count, 2);
    ASSERT_NE((*threads)[0], nullptr);
    ASSERT_NE((*threads)[1], nullptr);

    group->interrupt();
    first->join(5000);
    second->join(5000);

    EXPECT_TRUE(firstInterrupted.load());
    EXPECT_TRUE(secondInterrupted.load());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupTest, GroupInterruptPropagatesToMembers) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("interruptible"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(started, interrupted)),
        jxx::NEW<String>("member"));

    thread->start();
    waitUntil(started);
    group->interrupt();
    thread->join(5000);

    EXPECT_TRUE(interrupted.load());
    EXPECT_FALSE(thread->isAlive());
}

TEST(ThreadGroupTest, DaemonAndStringRepresentationAreObservable) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("diagnostic"));

    group->setDaemon(true);

    EXPECT_TRUE(group->isDaemon());
    EXPECT_NE(group->toString()->utf8().find("diagnostic"),
              std::string::npos);
}

TEST(ThreadGroupInterruptionTest, ParentInterruptReachesChildGroupThreads) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("parent-interrupt"));
    auto child = jxx::NEW<ThreadGroup>(parent, jxx::NEW<String>("child-interrupt"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        child,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(started, interrupted)),
        jxx::NEW<String>("child-member"));

    thread->start();
    waitUntil(started);
    parent->interrupt();
    thread->join(5000);

    EXPECT_TRUE(interrupted.load());
    EXPECT_FALSE(thread->isAlive());
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
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(
                started,
                interrupted,
                &statusCleared)),
        jxx::NEW<String>("clear-status-member"));

    thread->start();
    waitUntil(started);
    group->interrupt();
    thread->join(5000);

    EXPECT_TRUE(interrupted.load());
    EXPECT_TRUE(statusCleared.load());
}

TEST(ThreadGroupInterruptionTest, InterruptDoesNotMarkUnstartedMember) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("unstarted"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(started, interrupted)),
        jxx::NEW<String>("not-started"));

    group->interrupt();

    EXPECT_FALSE(thread->isInterrupted());
    EXPECT_EQ(thread->getState(), Thread::State::NEW);
}

TEST(ThreadGroupInterruptionTest, RepeatedInterruptAfterTerminationIsSafe) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("repeat"));
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(started, interrupted)),
        jxx::NEW<String>("repeat-member"));

    thread->start();
    waitUntil(started);
    group->interrupt();
    thread->join(5000);

    EXPECT_NO_THROW(group->interrupt());
    EXPECT_TRUE(interrupted.load());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, DaemonGroupInterruptsActiveMember) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("daemon-interrupt"));
    group->setDaemon(true);
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(started, interrupted)),
        jxx::NEW<String>("daemon-member"));

    thread->start();
    waitUntil(started);
    group->interrupt();
    thread->join(5000);

    EXPECT_TRUE(group->isDaemon());
    EXPECT_TRUE(interrupted.load());
    EXPECT_FALSE(thread->isAlive());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, EmptyDaemonGroupInterruptIsSafe) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("daemon-parent"));
    auto group = jxx::NEW<ThreadGroup>(parent, jxx::NEW<String>("empty-daemon"));
    group->setDaemon(true);

    EXPECT_NO_THROW(group->interrupt());
    EXPECT_TRUE(group->isDaemon());
    EXPECT_EQ(group->activeCount(), 0);
}

TEST(ThreadGroupInterruptionTest, TerminatedMembersAreIgnored) {
    auto group = jxx::NEW<ThreadGroup>(jxx::NEW<String>("terminated-members"));
    std::atomic<bool> ran{false};
    std::atomic<bool> unused{false};
    auto thread = jxx::NEW<Thread>(
        group,
        jxx::CAST<Runnable>(
            jxx::NEW<GroupSleepingRunnable>(ran, unused)),
        jxx::NEW<String>("terminated-member"));

    thread->start();
    waitUntil(ran);
    thread->interrupt();
    thread->join(5000);
    ASSERT_FALSE(thread->isAlive());
    ASSERT_EQ(group->activeCount(), 0);

    EXPECT_NO_THROW(group->interrupt());
    EXPECT_FALSE(thread->isInterrupted());
    EXPECT_EQ(thread->getState(), Thread::State::TERMINATED);
}

TEST(ThreadGroupInterruptionTest, DestroyedEmptyGroupRejectsNoOpInterruptSafely) {
    auto parent = jxx::NEW<ThreadGroup>(jxx::NEW<String>("destroy-parent"));
    auto group = jxx::NEW<ThreadGroup>(parent, jxx::NEW<String>("destroyed"));
    group->destroy();

    ASSERT_TRUE(group->isDestroyed());
    EXPECT_NO_THROW(group->interrupt());
    EXPECT_EQ(group->activeCount(), 0);
}
} // namespace
