#include <gtest/gtest.h>

#include <atomic>
#include <chrono>
#include <thread>

#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Timer.h"
#include "util/jxx.util.TimerTask.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace {

class DerivedTimer final
    : public ::jxx::lang::ClassBase<DerivedTimer, ::jxx::util::Timer> {
public:
    using JxxSuper = ::jxx::util::Timer;
    using Super = ::jxx::lang::ClassBase<DerivedTimer, JxxSuper>;

    explicit DerivedTimer(const ::jxx::Ptr<::jxx::lang::String>& name)
        : Super(name) {}
};

class CountingTask final : public ::jxx::util::TimerTask {
public:
    explicit CountingTask(std::atomic<int>& count) : count_(count) {}
    void run() override { ++count_; }
private:
    std::atomic<int>& count_;
};

TEST(TimerParity, SupportsApplicationSubclassAndOneShotSchedule)
{
    ::jxx::initialize();
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<DerivedTimer>(
        ::jxx::NEW<::jxx::lang::String>("AckManager-send"));
    timer->schedule(::jxx::NEW<CountingTask>(count), 0);
    for (int attempt = 0; attempt < 100 && count.load() == 0; ++attempt)
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    EXPECT_EQ(count.load(), 1);
    timer->cancel();
}

TEST(TimerParity, SupportsApplicationSubclassAndRepeatedSchedule)
{
    ::jxx::initialize();
    std::atomic<int> count{0};
    auto timer = ::jxx::NEW<DerivedTimer>(
        ::jxx::NEW<::jxx::lang::String>("AckManager-retry"));
    timer->schedule(::jxx::NEW<CountingTask>(count), 0, 5);
    for (int attempt = 0; attempt < 100 && count.load() < 2; ++attempt)
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
    EXPECT_GE(count.load(), 2);
    timer->cancel();
}

} // namespace
