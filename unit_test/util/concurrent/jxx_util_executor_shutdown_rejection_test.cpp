#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
class CountTask final : public ::jxx::lang::Runnable {
public: explicit CountTask(std::atomic<int>& c):c_(c){} void run() override{++c_;}
private: std::atomic<int>& c_;
};
TEST(ExecutorShutdownParityTest, OrderlyShutdownExecutesAcceptedTasksAndRejectsNewOnes) {
 std::atomic<int> count{0}; auto executor=::jxx::util::concurrent::Executors::newFixedThreadPool(1);
 executor->execute(::jxx::NEW<CountTask>(count)); executor->shutdown();
 EXPECT_TRUE(executor->isShutdown());
 EXPECT_THROW(executor->execute(::jxx::NEW<CountTask>(count)),::jxx::util::concurrent::RejectedExecutionException);
 EXPECT_TRUE(executor->awaitTermination(2,::jxx::util::concurrent::TimeUnit::SECONDS()));
 EXPECT_TRUE(executor->isTerminated()); EXPECT_EQ(1,count.load());
}
TEST(ExecutorShutdownParityTest, ShutdownNowReturnsWaitingTasksAndRejectsNewOnes) {
 std::atomic<int> count{0}; auto executor=::jxx::util::concurrent::Executors::newFixedThreadPool(1);
 const auto pending=executor->shutdownNow();
 EXPECT_TRUE(executor->isShutdown()); ASSERT_NE(nullptr,pending);
 EXPECT_THROW(executor->execute(::jxx::NEW<CountTask>(count)),::jxx::util::concurrent::RejectedExecutionException);
}
}
