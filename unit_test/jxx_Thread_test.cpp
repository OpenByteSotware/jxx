#include <atomic>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"

namespace {

using jxx::lang::Runnable;
using jxx::lang::String;
using jxx::lang::Thread;

class CountingRunnable final
    : public jxx::lang::ClassBase<
          CountingRunnable,
          jxx::lang::Object,
          Runnable> {
public:
    explicit CountingRunnable(
        std::atomic<int>& count)
        : count_(count) {
    }

    void run() override {
        ++count_;
    }

private:
    std::atomic<int>& count_;
};

TEST(ThreadTest, RunnableExecutesOnce) {
    std::atomic<int> count{0};

    auto target =
        jxx::NEW<CountingRunnable>(count);

    auto thread =
        jxx::NEW<Thread>(
            jxx::CAST<Runnable>(target));

    thread->start();
    thread->join();

    EXPECT_EQ(count.load(), 1);
    EXPECT_FALSE(thread->isAlive());
    EXPECT_EQ(
        thread->getState(),
        Thread::State::TERMINATED);
}

TEST(ThreadTest, ThisPtrKeepsThreadAliveDuringExecution) {
    std::atomic<bool> entered{false};
    std::atomic<bool> release{false};

    class WaitingRunnable final
        : public jxx::lang::ClassBase<
              WaitingRunnable,
              jxx::lang::Object,
              Runnable> {
    public:
        WaitingRunnable(
            std::atomic<bool>& entered,
            std::atomic<bool>& release)
            : entered_(entered)
            , release_(release) {
        }

        void run() override {
            entered_.store(true);
            while (!release_.load()) {
                std::this_thread::yield();
            }
        }

    private:
        std::atomic<bool>& entered_;
        std::atomic<bool>& release_;
    };

    auto target =
        jxx::NEW<WaitingRunnable>(
            entered,
            release);

    auto thread =
        jxx::NEW<Thread>(
            jxx::CAST<Runnable>(target));

    std::weak_ptr<Thread> observer = thread;

    thread->start();

    while (!entered.load()) {
        std::this_thread::yield();
    }

    thread.reset();

    EXPECT_FALSE(observer.expired());

    release.store(true);

    auto retained = observer.lock();
    ASSERT_NE(retained, nullptr);
    retained->join();
    retained.reset();

    EXPECT_TRUE(observer.expired());
}

TEST(ThreadTest, StartTwiceThrows) {
    auto thread = jxx::NEW<Thread>();

    thread->start();
    thread->join();

    EXPECT_THROW(
        thread->start(),
        jxx::lang::IllegalStateException);
}

TEST(ThreadTest, NamePriorityAndDaemonRoundTrip) {
    auto thread = jxx::NEW<Thread>();

    thread->setName(
        jxx::NEW<String>("worker"));
    thread->setPriority(7);
    thread->setDaemon(true);

    EXPECT_EQ(thread->getName()->utf8(), "worker");
    EXPECT_EQ(thread->getPriority(), 7);
    EXPECT_TRUE(thread->isDaemon());
}

TEST(ThreadTest, PriorityRangeIsValidated) {
    auto thread = jxx::NEW<Thread>();

    EXPECT_THROW(
        thread->setPriority(0),
        jxx::lang::IllegalArgumentException);

    EXPECT_THROW(
        thread->setPriority(11),
        jxx::lang::IllegalArgumentException);
}

TEST(ThreadTest, CurrentThreadIsAvailableInsideRun) {
    std::atomic<bool> found{false};

    class CurrentRunnable final
        : public jxx::lang::ClassBase<
              CurrentRunnable,
              jxx::lang::Object,
              Runnable> {
    public:
        explicit CurrentRunnable(
            std::atomic<bool>& found)
            : found_(found) {
        }

        void run() override {
            found_.store(
                Thread::currentThread() != nullptr);
        }

    private:
        std::atomic<bool>& found_;
    };

    auto target =
        jxx::NEW<CurrentRunnable>(found);

    auto thread =
        jxx::NEW<Thread>(
            jxx::CAST<Runnable>(target));

    thread->start();
    thread->join();

    EXPECT_TRUE(found.load());
}

TEST(ThreadTest, InterruptStatusCanBeObserved) {
    auto thread = jxx::NEW<Thread>();

    EXPECT_FALSE(thread->isInterrupted());

    thread->interrupt();

    EXPECT_TRUE(thread->isInterrupted());
}

} // namespace
