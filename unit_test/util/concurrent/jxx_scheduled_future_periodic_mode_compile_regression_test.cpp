#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
namespace {
class NoOp final : public ::jxx::lang::ClassBase<NoOp, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public: void run() override {}
};
TEST(ScheduledFuturePeriodicModeRegression, ExposesAllSchedulerModes) {
    using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;
    const auto command = ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>());
    EXPECT_NE(nullptr, ::jxx::NEW<Task>(command, nullptr, 0, 0, Task::PeriodicMode::OneShot));
    EXPECT_NE(nullptr, ::jxx::NEW<Task>(command, nullptr, 0, 1, Task::PeriodicMode::FixedRate));
    EXPECT_NE(nullptr, ::jxx::NEW<Task>(command, nullptr, 0, 1, Task::PeriodicMode::FixedDelay));
}
}
