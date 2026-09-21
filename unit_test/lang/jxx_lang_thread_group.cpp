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
                          std::atomic<bool>& interrupted)
        : started_(started), interrupted_(interrupted) {}

    void run() override {
        started_.store(true);
        try {
            Thread::sleep(30000);
        }
        catch (const jxx::lang::InterruptedException&) {
            interrupted_.store(true);
        }
    }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& interrupted_;
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
} // namespace
