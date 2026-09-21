#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Throwable.h"

namespace {
using namespace std::chrono_literals;
using jxx::lang::Object;
using jxx::lang::Runnable;
using jxx::lang::String;
using jxx::lang::Thread;
using jxx::lang::Throwable;

void waitUntil(const std::atomic<bool>& value) {
    const auto deadline = std::chrono::steady_clock::now() + 5s;
    while (!value.load() && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }
    ASSERT_TRUE(value.load());
}

class RecordingRunnable final
    : public jxx::lang::ClassBase<RecordingRunnable, Object, Runnable> {
public:
    RecordingRunnable(std::atomic<bool>& ran,
                      std::atomic<bool>& sawCurrentThread)
        : ran_(ran), sawCurrentThread_(sawCurrentThread) {}

    void run() override {
        sawCurrentThread_.store(Thread::currentThread() != nullptr);
        ran_.store(true);
    }

private:
    std::atomic<bool>& ran_;
    std::atomic<bool>& sawCurrentThread_;
};

class SleepingRunnable final
    : public jxx::lang::ClassBase<SleepingRunnable, Object, Runnable> {
public:
    SleepingRunnable(std::atomic<bool>& started,
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

class ThrowingRunnable final
    : public jxx::lang::ClassBase<ThrowingRunnable, Object, Runnable> {
public:
    void run() override {
        throw jxx::lang::IllegalStateException(
            jxx::NEW<String>("uncaught"));
    }
};

class RecordingHandler final
    : public jxx::lang::ClassBase<
          RecordingHandler,
          Object,
          Thread::UncaughtExceptionHandler> {
public:
    void uncaughtException(
        const jxx::Ptr<Thread>& thread,
        const jxx::Ptr<Throwable>& throwable) override {
        threadSeen_.store(thread != nullptr);
        throwableSeen_.store(throwable != nullptr);
        calls_.fetch_add(1);
    }

    int calls() const { return calls_.load(); }
    bool threadSeen() const { return threadSeen_.load(); }
    bool throwableSeen() const { return throwableSeen_.load(); }

private:
    std::atomic<int> calls_{0};
    std::atomic<bool> threadSeen_{false};
    std::atomic<bool> throwableSeen_{false};
};

TEST(ThreadTest, DefaultsAndMutableAttributesMatchContract) {
    auto thread = jxx::NEW<Thread>();

    EXPECT_EQ(thread->getState(), Thread::State::NEW);
    EXPECT_FALSE(thread->isAlive());
    EXPECT_FALSE(thread->isDaemon());
    EXPECT_EQ(thread->getPriority(), Thread::NORM_PRIORITY);

    thread->setName(jxx::NEW<String>("worker"));
    thread->setPriority(Thread::MAX_PRIORITY);
    thread->setDaemon(true);

    EXPECT_EQ(thread->getName()->utf8(), "worker");
    EXPECT_LE(thread->getPriority(), Thread::MAX_PRIORITY);
    EXPECT_TRUE(thread->isDaemon());
}

TEST(ThreadTest, StartRunsTargetAndJoinObservesTermination) {
    std::atomic<bool> ran{false};
    std::atomic<bool> sawCurrentThread{false};
    auto target = jxx::NEW<RecordingRunnable>(ran, sawCurrentThread);
    auto thread = jxx::NEW<Thread>(
        jxx::CAST<Runnable>(target),
        jxx::NEW<String>("runner"));

    thread->start();
    thread->join();

    EXPECT_TRUE(ran.load());
    EXPECT_TRUE(sawCurrentThread.load());
    EXPECT_FALSE(thread->isAlive());
    EXPECT_EQ(thread->getState(), Thread::State::TERMINATED);
    EXPECT_EQ(thread->getThreadGroup(), nullptr);
}

TEST(ThreadTest, StartingTwiceThrowsIllegalStateException) {
    std::atomic<bool> ran{false};
    std::atomic<bool> sawCurrentThread{false};
    auto thread = jxx::NEW<Thread>(
        jxx::CAST<Runnable>(
            jxx::NEW<RecordingRunnable>(ran, sawCurrentThread)));

    thread->start();
    EXPECT_THROW(thread->start(), jxx::lang::IllegalStateException);
    thread->join();
}

TEST(ThreadTest, InterruptWakesSleepingThread) {
    std::atomic<bool> started{false};
    std::atomic<bool> interrupted{false};
    auto thread = jxx::NEW<Thread>(
        jxx::CAST<Runnable>(
            jxx::NEW<SleepingRunnable>(started, interrupted)));

    thread->start();
    waitUntil(started);
    thread->interrupt();
    thread->join(5000);

    EXPECT_TRUE(interrupted.load());
    EXPECT_FALSE(thread->isAlive());
}

TEST(ThreadTest, ExplicitUncaughtExceptionHandlerReceivesThrowable) {
    auto handler = jxx::NEW<RecordingHandler>();
    auto thread = jxx::NEW<Thread>(
        jxx::CAST<Runnable>(jxx::NEW<ThrowingRunnable>()));
    thread->setUncaughtExceptionHandler(
        jxx::CAST<Thread::UncaughtExceptionHandler>(handler));

    thread->start();
    thread->join();

    EXPECT_EQ(handler->calls(), 1);
    EXPECT_TRUE(handler->threadSeen());
    EXPECT_TRUE(handler->throwableSeen());
}

TEST(ThreadTest, DefaultUncaughtExceptionHandlerCanBeReplacedAndRestored) {
    const auto previous = Thread::getDefaultUncaughtExceptionHandler();
    auto handler = jxx::NEW<RecordingHandler>();

    Thread::setDefaultUncaughtExceptionHandler(
        jxx::CAST<Thread::UncaughtExceptionHandler>(handler));
    EXPECT_EQ(Thread::getDefaultUncaughtExceptionHandler().get(),
              handler.get());

    Thread::setDefaultUncaughtExceptionHandler(previous);
    EXPECT_EQ(Thread::getDefaultUncaughtExceptionHandler().get(),
              previous.get());
}

TEST(ThreadTest, InvalidPriorityAndNullNameAreRejected) {
    auto thread = jxx::NEW<Thread>();

    EXPECT_THROW(thread->setPriority(Thread::MIN_PRIORITY - 1),
                 jxx::lang::IllegalArgumentException);
    EXPECT_THROW(thread->setPriority(Thread::MAX_PRIORITY + 1),
                 jxx::lang::IllegalArgumentException);
    EXPECT_THROW(thread->setName(nullptr),
                 jxx::lang::NullPointerException);
}
} // namespace
