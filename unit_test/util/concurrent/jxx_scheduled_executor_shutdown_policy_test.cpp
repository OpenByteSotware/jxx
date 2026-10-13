#include <gtest/gtest.h>

#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"

#include <functional>

namespace {
class TestRunnable final
    : public ::jxx::lang::ClassBase<
          TestRunnable, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    explicit TestRunnable(std::function<void()> function)
        : function_(std::move(function)) {}
    void run() override { function_(); }
private:
    std::function<void()> function_;
};

::jxx::Ptr<::jxx::lang::Runnable> testRunnable(std::function<void()> function) {
    return ::jxx::CAST<::jxx::lang::Runnable>(
        ::jxx::NEW<TestRunnable>(std::move(function)));
}
} // namespace

TEST(JxxScheduledExecutorShutdownPolicyTest,
     PolicyChangeCancelsDelayedTaskWithoutRecursiveLocking) {
    const auto executor =
        ::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(4);
    const auto future = executor->schedule(
        testRunnable([] {}),
        1,
        ::jxx::util::concurrent::TimeUnit::HOURS());

    executor->shutdown();
    EXPECT_NO_THROW(
        executor->setExecuteExistingDelayedTasksAfterShutdownPolicy(false));

    EXPECT_TRUE(future->isCancelled());
    EXPECT_TRUE(future->isDone());
}
