#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class Callback final
    : public ::jxx::lang::ClassBase<
          Callback,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        Callback, JxxSuper, ::jxx::lang::Runnable>;

    explicit Callback(std::function<void()> callback)
        : callback_(std::move(callback)) {}

    void run() override { callback_(); }

private:
    std::function<void()> callback_;
};

bool waitDone(
    const ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<
        ::jxx::lang::Object>>& future) {
    for (int retry = 0; retry < 200 && !future->isDone(); ++retry) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
    return future->isDone();
}
}

TEST(TimersSourceParity, ZeroDelayTaskRuns) {
    auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
    std::atomic<int> count{0};
    auto future = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<Callback>([&] { ++count; })),
        0,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    EXPECT_TRUE(waitDone(future));
    EXPECT_EQ(1, count.load());
    executor->shutdownNow();
}

TEST(TimersSourceParity, IndependentFutureFieldsCancelIndependently) {
    auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
    std::atomic<int> firstCount{0};
    std::atomic<int> secondCount{0};

    auto first = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<Callback>([&] { ++firstCount; })),
        300,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());
    auto second = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<Callback>([&] { ++secondCount; })),
        1,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    EXPECT_TRUE(first->cancel(false));
    EXPECT_TRUE(waitDone(second));
    EXPECT_EQ(0, firstCount.load());
    EXPECT_EQ(1, secondCount.load());
    executor->shutdownNow();
}

TEST(TimersSourceParity, TaskCanScheduleReplacementFromRun) {
    auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
    std::atomic<int> count{0};
    ::jxx::Ptr<::jxx::util::concurrent::ScheduledFuture<
        ::jxx::lang::Object>> replacement;

    auto first = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(
            ::jxx::NEW<Callback>([&] {
                ++count;
                replacement = executor->schedule(
                    ::jxx::CAST<::jxx::lang::Runnable>(
                        ::jxx::NEW<Callback>([&] { ++count; })),
                    1,
                    ::jxx::util::concurrent::TimeUnit::MILLISECONDS());
            })),
        0,
        ::jxx::util::concurrent::TimeUnit::MILLISECONDS());

    EXPECT_TRUE(waitDone(first));
    for (int retry = 0; retry < 200 && replacement == nullptr; ++retry) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    ASSERT_NE(nullptr, replacement);
    EXPECT_TRUE(waitDone(replacement));
    EXPECT_EQ(2, count.load());
    executor->shutdownNow();
}
