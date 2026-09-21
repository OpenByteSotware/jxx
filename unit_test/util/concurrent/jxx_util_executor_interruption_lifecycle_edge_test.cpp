#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.CountDownLatch.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {

class InterruptibleWait final : public ::jxx::lang::Runnable {
public:
    InterruptibleWait(
        const ::jxx::Ptr<::jxx::util::concurrent::CountDownLatch>& entered,
        const ::jxx::Ptr<::jxx::util::concurrent::CountDownLatch>& blocker,
        std::atomic<bool>& interrupted)
        : entered_(entered), blocker_(blocker), interrupted_(interrupted) {}

    void run() override {
        entered_->countDown();
        try {
            blocker_->await();
        }
        catch (const ::jxx::lang::InterruptedException&) {
            interrupted_.store(true, std::memory_order_release);
        }
    }

private:
    ::jxx::Ptr<::jxx::util::concurrent::CountDownLatch> entered_;
    ::jxx::Ptr<::jxx::util::concurrent::CountDownLatch> blocker_;
    std::atomic<bool>& interrupted_;
};

class BlockingTask final : public ::jxx::lang::Runnable {
public:
    BlockingTask(
        std::mutex& mutex,
        std::condition_variable& condition,
        bool& entered,
        bool& release)
        : mutex_(mutex), condition_(condition), entered_(entered), release_(release) {}

    void run() override {
        std::unique_lock<std::mutex> lock(mutex_);
        entered_ = true;
        condition_.notify_all();
        condition_.wait(lock, [&] { return release_; });
    }

private:
    std::mutex& mutex_;
    std::condition_variable& condition_;
    bool& entered_;
    bool& release_;
};

class CountTask final : public ::jxx::lang::Runnable {
public:
    explicit CountTask(std::atomic<int>& count) : count_(count) {}
    void run() override { count_.fetch_add(1, std::memory_order_relaxed); }
private:
    std::atomic<int>& count_;
};

TEST(InterruptionParityTest, InterruptWakesInterruptibleLatchWaitAndClearsStatus) {
    const auto entered = ::jxx::NEW<::jxx::util::concurrent::CountDownLatch>(1);
    const auto blocker = ::jxx::NEW<::jxx::util::concurrent::CountDownLatch>(1);
    std::atomic<bool> interrupted{false};
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::NEW<InterruptibleWait>(entered, blocker, interrupted));

    thread->start();
    ASSERT_TRUE(entered->await(2, ::jxx::util::concurrent::TimeUnit::SECONDS()));
    thread->interrupt();
    thread->join(2000);

    EXPECT_TRUE(interrupted.load(std::memory_order_acquire));
    EXPECT_FALSE(thread->isAlive());
    EXPECT_FALSE(thread->isInterrupted());
}

TEST(ExecutorLifecycleEdgeTest, AwaitTerminationTimesOutBeforeShutdown) {
    const auto executor = ::jxx::util::concurrent::Executors::newSingleThreadExecutor();
    EXPECT_FALSE(executor->isShutdown());
    EXPECT_FALSE(executor->isTerminated());
    EXPECT_FALSE(executor->awaitTermination(
        0, ::jxx::util::concurrent::TimeUnit::NANOSECONDS()));
    executor->shutdown();
    EXPECT_TRUE(executor->awaitTermination(
        2, ::jxx::util::concurrent::TimeUnit::SECONDS()));
    EXPECT_TRUE(executor->isTerminated());
}

TEST(ExecutorLifecycleEdgeTest, RepeatedShutdownIsIdempotentAndSubmissionsReject) {
    const auto executor = ::jxx::util::concurrent::Executors::newFixedThreadPool(1);
    std::atomic<int> count{0};
    executor->execute(::jxx::NEW<CountTask>(count));
    executor->shutdown();
    executor->shutdown();

    EXPECT_TRUE(executor->isShutdown());
    EXPECT_THROW(
        executor->execute(::jxx::NEW<CountTask>(count)),
        ::jxx::util::concurrent::RejectedExecutionException);
    EXPECT_TRUE(executor->awaitTermination(
        2, ::jxx::util::concurrent::TimeUnit::SECONDS()));
    EXPECT_EQ(1, count.load(std::memory_order_relaxed));
}

TEST(ExecutorLifecycleEdgeTest, ShutdownNowReturnsQueuedTasksWithoutRunningThem) {
    const auto executor = ::jxx::util::concurrent::Executors::newFixedThreadPool(1);
    std::mutex mutex;
    std::condition_variable condition;
    bool entered = false;
    bool release = false;
    std::atomic<int> queuedRuns{0};

    executor->execute(::jxx::NEW<BlockingTask>(mutex, condition, entered, release));
    {
        std::unique_lock<std::mutex> lock(mutex);
        ASSERT_TRUE(condition.wait_for(
            lock, std::chrono::seconds(2), [&] { return entered; }));
    }
    executor->execute(::jxx::NEW<CountTask>(queuedRuns));
    executor->execute(::jxx::NEW<CountTask>(queuedRuns));

    const auto waiting = executor->shutdownNow();
    ASSERT_NE(nullptr, waiting);
    EXPECT_EQ(2, waiting->size());
    EXPECT_EQ(0, queuedRuns.load(std::memory_order_relaxed));

    {
        std::lock_guard<std::mutex> lock(mutex);
        release = true;
    }
    condition.notify_all();
    EXPECT_TRUE(executor->awaitTermination(
        2, ::jxx::util::concurrent::TimeUnit::SECONDS()));
    EXPECT_TRUE(executor->isTerminated());
}

} // namespace
