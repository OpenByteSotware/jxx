#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class CountTask final
    : public ::jxx::lang::ClassBase<
          CountTask,
          ::jxx::lang::Object,
          ::jxx::lang::Runnable> {
public:
    explicit CountTask(std::atomic<int>& count) : count_(count) {}
    void run() override { count_.fetch_add(1, std::memory_order_relaxed); }
private:
    std::atomic<int>& count_;
};

TEST(ExecutorLifecycleEdgeParity, RepeatedShutdownRejectsNewTasks) {
    const auto executor =
        ::jxx::util::concurrent::Executors::newFixedThreadPool(1);
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
} // namespace
