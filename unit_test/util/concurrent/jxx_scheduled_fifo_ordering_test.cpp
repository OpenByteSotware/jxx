#include <gtest/gtest.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <vector>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {

class OrderedRunnable final
    : public ::jxx::lang::ClassBase<
          OrderedRunnable, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    OrderedRunnable(
        int index,
        std::vector<int>& order,
        std::mutex& mutex,
        std::condition_variable& changed)
        : index_(index), order_(order), mutex_(mutex), changed_(changed) {}

    void run() override {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            order_.push_back(index_);
        }
        changed_.notify_all();
    }

private:
    int index_;
    std::vector<int>& order_;
    std::mutex& mutex_;
    std::condition_variable& changed_;
};

TEST(ScheduledFifoOrderingParity, EqualDeadlineTasksUseSubmissionSequence) {
    using ::jxx::util::concurrent::ScheduledFutureTask;

    std::vector<int> unusedOrder;
    std::mutex unusedMutex;
    std::condition_variable unusedChanged;
    auto runnable = ::jxx::CAST<::jxx::lang::Runnable>(
        ::jxx::NEW<OrderedRunnable>(
            0, unusedOrder, unusedMutex, unusedChanged));

    auto first = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        runnable, nullptr, 1000000000LL, 0,
        ScheduledFutureTask<::jxx::lang::Object>::PeriodicMode::OneShot);
    auto second = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        runnable, nullptr, 1000000000LL, 0,
        ScheduledFutureTask<::jxx::lang::Object>::PeriodicMode::OneShot);

    // Different construction instants normally produce different deadlines.
    // The stable sequence key must nevertheless be monotonic and independent
    // of wall-clock-relative getDelay() calls.
    EXPECT_LT(first->sequenceNumber(), second->sequenceNumber());
    EXPECT_LE(first->compareTo(
        ::jxx::CAST<::jxx::util::concurrent::Delayed>(second)), 0);
}

TEST(ScheduledFifoOrderingParity, SubmittedTasksExecuteInOrderOnSingleWorker) {
    using ::jxx::util::concurrent::ScheduledThreadPoolExecutor;
    using ::jxx::util::concurrent::TimeUnit;

    const auto executor = ::jxx::NEW<ScheduledThreadPoolExecutor>(1);
    std::vector<int> order;
    std::mutex mutex;
    std::condition_variable changed;

    for (int index = 0; index < 16; ++index) {
        executor->schedule(
            ::jxx::CAST<::jxx::lang::Runnable>(
                ::jxx::NEW<OrderedRunnable>(
                    index, order, mutex, changed)),
            100,
            TimeUnit::MILLISECONDS());
    }

    {
        std::unique_lock<std::mutex> lock(mutex);
        ASSERT_TRUE(changed.wait_for(
            lock, std::chrono::seconds(3), [&] { return order.size() == 16U; }));
    }

    (void)executor->shutdownNow();
    for (int index = 0; index < 16; ++index) {
        EXPECT_EQ(index, order[static_cast<std::size_t>(index)]);
    }
}

} // namespace
