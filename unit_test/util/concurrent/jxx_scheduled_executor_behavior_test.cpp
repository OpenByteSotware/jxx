#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.RuntimeException.h"
#include "util/concurrent/jxx.util.concurrent.CancellationException.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFuture.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"

namespace {

using ::jxx::lang::Object;
using ::jxx::lang::Runnable;
using ::jxx::util::concurrent::CancellationException;
using ::jxx::util::concurrent::Executors;
using ::jxx::util::concurrent::ScheduledExecutorService;
using ::jxx::util::concurrent::ScheduledFuture;
using ::jxx::util::concurrent::TimeUnit;

bool waitUntil(const std::function<bool()>& predicate,
               std::chrono::milliseconds timeout) {
    const auto deadline = std::chrono::steady_clock::now() + timeout;
    while (std::chrono::steady_clock::now() < deadline) {
        if (predicate()) return true;
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    return predicate();
}

class CountingRunnable final
    : public ::jxx::lang::ClassBase<CountingRunnable, Object, Runnable> {
public:
    explicit CountingRunnable(std::atomic<int>& count) : count_(count) {}

    void run() override { count_.fetch_add(1, std::memory_order_relaxed); }

private:
    std::atomic<int>& count_;
};

class FailingRunnable final
    : public ::jxx::lang::ClassBase<FailingRunnable, Object, Runnable> {
public:
    explicit FailingRunnable(std::atomic<int>& count) : count_(count) {}

    void run() override {
        count_.fetch_add(1, std::memory_order_relaxed);
        throw ::jxx::lang::RuntimeException();
    }

private:
    std::atomic<int>& count_;
};

class NonOverlappingRunnable final
    : public ::jxx::lang::ClassBase<NonOverlappingRunnable, Object, Runnable> {
public:
    NonOverlappingRunnable(std::atomic<int>& count,
                           std::atomic<int>& active,
                           std::atomic<int>& maximumActive)
        : count_(count), active_(active), maximumActive_(maximumActive) {}

    void run() override {
        const int activeNow = active_.fetch_add(1, std::memory_order_acq_rel) + 1;
        int observed = maximumActive_.load(std::memory_order_relaxed);
        while (observed < activeNow &&
               !maximumActive_.compare_exchange_weak(
                   observed, activeNow, std::memory_order_relaxed)) {}
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
        count_.fetch_add(1, std::memory_order_relaxed);
        active_.fetch_sub(1, std::memory_order_acq_rel);
    }

private:
    std::atomic<int>& count_;
    std::atomic<int>& active_;
    std::atomic<int>& maximumActive_;
};

class ScheduledExecutorBehaviorTest : public ::testing::Test {
protected:
    void SetUp() override {
        scheduler_ = Executors::newScheduledThreadPool(2);
    }

    void TearDown() override {
        if (scheduler_ != nullptr) (void)scheduler_->shutdownNow();
    }

    ::jxx::Ptr<ScheduledExecutorService> scheduler_;
};

TEST_F(ScheduledExecutorBehaviorTest, NegativeOneShotDelayRunsImmediately) {
    std::atomic<int> count{0};
    auto task = ::jxx::NEW<CountingRunnable>(count);
    auto future = scheduler_->schedule(
        ::jxx::CAST<Runnable>(task), -1, TimeUnit::DAYS());

    (void)future->get(2, TimeUnit::SECONDS());
    EXPECT_EQ(1, count.load(std::memory_order_relaxed));
    EXPECT_TRUE(future->isDone());
}

TEST_F(ScheduledExecutorBehaviorTest, PeriodicFailureSuppressesLaterExecutions) {
    std::atomic<int> count{0};
    auto task = ::jxx::NEW<FailingRunnable>(count);
    auto future = scheduler_->scheduleAtFixedRate(
        ::jxx::CAST<Runnable>(task), 0, 1, TimeUnit::MILLISECONDS());

    ASSERT_TRUE(waitUntil([&] { return future->isDone(); },
                          std::chrono::milliseconds(2000)));
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
    EXPECT_EQ(1, count.load(std::memory_order_relaxed));
    EXPECT_FALSE(future->isCancelled());
}

TEST_F(ScheduledExecutorBehaviorTest, FixedRateExecutionsDoNotOverlap) {
    std::atomic<int> count{0};
    std::atomic<int> active{0};
    std::atomic<int> maximumActive{0};
    auto task = ::jxx::NEW<NonOverlappingRunnable>(count, active, maximumActive);
    auto future = scheduler_->scheduleAtFixedRate(
        ::jxx::CAST<Runnable>(task), 0, 1, TimeUnit::MILLISECONDS());

    ASSERT_TRUE(waitUntil([&] {
        return count.load(std::memory_order_relaxed) >= 3;
    }, std::chrono::milliseconds(2000)));
    EXPECT_TRUE(future->cancel(true));
    EXPECT_EQ(1, maximumActive.load(std::memory_order_relaxed));
}

TEST_F(ScheduledExecutorBehaviorTest, CancellationIsTerminal) {
    std::atomic<int> count{0};
    auto task = ::jxx::NEW<CountingRunnable>(count);
    auto future = scheduler_->schedule(
        ::jxx::CAST<Runnable>(task), 1, TimeUnit::DAYS());

    EXPECT_TRUE(future->cancel(false));
    EXPECT_FALSE(future->cancel(false));
    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());
    EXPECT_THROW((void)future->get(), CancellationException);
    EXPECT_EQ(0, count.load(std::memory_order_relaxed));
}

TEST_F(ScheduledExecutorBehaviorTest, ShutdownRejectsNewScheduledTask) {
    scheduler_->shutdown();
    std::atomic<int> count{0};
    auto task = ::jxx::NEW<CountingRunnable>(count);

    EXPECT_THROW(
        (void)scheduler_->schedule(
            ::jxx::CAST<Runnable>(task), 0, TimeUnit::NANOSECONDS()),
        ::jxx::util::concurrent::RejectedExecutionException);
}

} // namespace
