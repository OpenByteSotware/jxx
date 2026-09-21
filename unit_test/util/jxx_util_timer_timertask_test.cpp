#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.IllegalStateException.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"

namespace {
class CountingTask final : public ::jxx::util::TimerTask {
public:
    explicit CountingTask(std::atomic<int>& count) : count_(count) {}
    void run() override { count_.fetch_add(1, std::memory_order_relaxed); }
private:
    std::atomic<int>& count_;
};

TEST(TimerParityTest, ExecutesOneShotTaskAndRecordsScheduledTime) {
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<::jxx::util::Timer>(true);
    auto task = ::jxx::NEW<CountingTask>(count);
    timer->schedule(task, 0);
    for (int i=0; i<100 && count.load()==0; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_EQ(1, count.load());
    EXPECT_TRUE(task->scheduledExecutionTime() > 0);
    timer->cancel();
}

TEST(TimerParityTest, RepeatingTaskStopsAfterTaskCancellation) {
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    auto task = ::jxx::NEW<CountingTask>(count);
    timer->scheduleAtFixedRate(task, 0, 2);
    for (int i=0; i<100 && count.load()<2; ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    EXPECT_TRUE(task->cancel());
    const auto observed = count.load();
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    EXPECT_EQ(observed, count.load());
    timer->cancel();
}

TEST(TimerParityTest, CancelledTimerRejectsNewTasks) {
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    std::atomic<int> count{0};
    timer->cancel();
    EXPECT_THROW(timer->schedule(::jxx::NEW<CountingTask>(count), 0),
                 ::jxx::lang::IllegalStateException);
}

TEST(TimerParityTest, PurgeRemovesCancelledWaitingTasks) {
    auto timer = ::jxx::NEW<::jxx::util::Timer>();
    std::atomic<int> count{0};
    auto first = ::jxx::NEW<CountingTask>(count);
    auto second = ::jxx::NEW<CountingTask>(count);
    timer->schedule(first, 10000);
    timer->schedule(second, 10000);
    EXPECT_TRUE(first->cancel());
    EXPECT_EQ(1, timer->purge());
    timer->cancel();
}
} // namespace
