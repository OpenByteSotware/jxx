#include <atomic>
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <thread>

#include <gtest/gtest.h>

#include "lang/jxx.lang.Exceptions.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledFuture.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {

using Clock = std::chrono::steady_clock;

class CountingPeriodicTask final
    : public ::jxx::lang::ClassBase<
          CountingPeriodicTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {
        const int active = active_.fetch_add(1) + 1;
        int observed = maxActive_.load();
        while (active > observed &&
               !maxActive_.compare_exchange_weak(observed, active)) {
        }

        const int invocation = invocations_.fetch_add(1) + 1;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            if (invocation >= 3) reachedThree_ = true;
        }
        condition_.notify_all();

        try {
            ::jxx::lang::Thread::sleep(25);
        }
        catch (const ::jxx::lang::InterruptedException&) {
            interrupted_.store(true);
        }
        active_.fetch_sub(1);
    }

    bool waitForThree(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        return condition_.wait_for(lock, timeout, [this] {
            return reachedThree_;
        });
    }

    int invocations() const { return invocations_.load(); }
    int maxActive() const { return maxActive_.load(); }

private:
    std::atomic<int> active_{0};
    std::atomic<int> maxActive_{0};
    std::atomic<int> invocations_{0};
    std::atomic<bool> interrupted_{false};
    std::mutex mutex_;
    std::condition_variable condition_;
    bool reachedThree_ = false;
};

class InterruptibleTask final
    : public ::jxx::lang::ClassBase<
          InterruptibleTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            started_ = true;
        }
        condition_.notify_all();

        try {
            ::jxx::lang::Thread::sleep(30000);
        }
        catch (const ::jxx::lang::InterruptedException&) {
            {
                std::lock_guard<std::mutex> lock(mutex_);
                interrupted_ = true;
            }
            condition_.notify_all();
        }
    }

    bool waitStarted(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        return condition_.wait_for(lock, timeout, [this] { return started_; });
    }

    bool waitInterrupted(std::chrono::milliseconds timeout) {
        std::unique_lock<std::mutex> lock(mutex_);
        return condition_.wait_for(lock, timeout, [this] { return interrupted_; });
    }

private:
    std::mutex mutex_;
    std::condition_variable condition_;
    bool started_ = false;
    bool interrupted_ = false;
};

class FailingPeriodicTask final
    : public ::jxx::lang::ClassBase<
          FailingPeriodicTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {
        invocations_.fetch_add(1);
        throw ::jxx::lang::RuntimeException("expected test failure");
    }

    int invocations() const { return invocations_.load(); }

private:
    std::atomic<int> invocations_{0};
};

class NoOpTask final
    : public ::jxx::lang::ClassBase<
          NoOpTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledExecutorParity, FixedRateTaskNeverOverlapsAndStopsAfterCancel) {
    const auto executor =
        ::jxx::util::concurrent::Executors::newScheduledThreadPool(2);
    const auto task = ::jxx::NEW<CountingPeriodicTask>();
    const auto future = executor->scheduleAtFixedRate(
        ::jxx::CAST<::jxx::lang::Runnable>(task),
        0,
        5,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    ASSERT_TRUE(task->waitForThree(std::chrono::seconds(3)));
    EXPECT_EQ(1, task->maxActive());

    ASSERT_TRUE(future->cancel(false));
    const int countAfterCancel = task->invocations();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(countAfterCancel, task->invocations());
    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());

    executor->shutdown();
    EXPECT_TRUE(executor->awaitTermination(
        3, ::jxx::util::concurrent::TimeUnit::SECONDS()));
}

TEST(ScheduledExecutorParity, CancelTrueInterruptsRunningJxxWorker) {
    const auto executor =
        ::jxx::util::concurrent::Executors::newScheduledThreadPool(1);
    const auto task = ::jxx::NEW<InterruptibleTask>();
    const auto future = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(task),
        0,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    ASSERT_TRUE(task->waitStarted(std::chrono::seconds(3)));
    ASSERT_TRUE(future->cancel(true));
    EXPECT_TRUE(task->waitInterrupted(std::chrono::seconds(3)));
    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());

    executor->shutdown();
    EXPECT_TRUE(executor->awaitTermination(
        3, ::jxx::util::concurrent::TimeUnit::SECONDS()));
}

TEST(ScheduledExecutorParity, PeriodicFailureSuppressesLaterExecutions) {
    const auto executor =
        ::jxx::util::concurrent::Executors::newScheduledThreadPool(1);
    const auto task = ::jxx::NEW<FailingPeriodicTask>();
    const auto future = executor->scheduleAtFixedRate(
        ::jxx::CAST<::jxx::lang::Runnable>(task),
        0,
        1,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    const auto deadline = Clock::now() + std::chrono::seconds(3);
    while (!future->isDone() && Clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    ASSERT_TRUE(future->isDone());
    EXPECT_FALSE(future->isCancelled());
    EXPECT_EQ(1, task->invocations());

    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    EXPECT_EQ(1, task->invocations());

    executor->shutdown();
    EXPECT_TRUE(executor->awaitTermination(
        3, ::jxx::util::concurrent::TimeUnit::SECONDS()));
}

TEST(ScheduledExecutorParity, ShutdownRejectsNewTasks) {
    const auto executor =
        ::jxx::util::concurrent::Executors::newScheduledThreadPool(1);
    executor->shutdown();

    EXPECT_THROW(
        executor->schedule(
            ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOpTask>()),
            0,
            ::jxx::util::concurrent::TimeUnit::MILLISECONDS()),
        ::jxx::util::concurrent::RejectedExecutionException);

    EXPECT_TRUE(executor->awaitTermination(
        3, ::jxx::util::concurrent::TimeUnit::SECONDS()));
}

} // namespace
