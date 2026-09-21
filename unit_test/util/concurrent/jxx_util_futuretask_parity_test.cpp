#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.concurrent.CancellationException.h"
#include "util/jxx.util.concurrent.FutureTask.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.TimeoutException.h"
namespace {
using S=::jxx::lang::String;
class Increment final:public ::jxx::lang::Runnable{public:explicit Increment(std::atomic<int>& v):v_(v){}void run()override{++v_;}private:std::atomic<int>& v_;};
TEST(FutureTaskParityTest, RunnableConstructorReturnsConfiguredResult){
 std::atomic<int> count{0}; auto result=::jxx::NEW<S>("done");
 auto task=::jxx::NEW<::jxx::util::concurrent::FutureTask<S>>(::jxx::NEW<Increment>(count),result);
 task->run(); EXPECT_EQ(1,count.load()); EXPECT_EQ(result.get(),task->get().get()); EXPECT_TRUE(task->isDone());
}
TEST(FutureTaskParityTest, ZeroTimeoutReportsIncompleteButReturnsCompleted){
 std::atomic<int> count{0}; auto task=::jxx::NEW<::jxx::util::concurrent::FutureTask<S>>(::jxx::NEW<Increment>(count),nullptr);
 EXPECT_THROW(task->get(0,::jxx::util::concurrent::TimeUnit::NANOSECONDS()),::jxx::util::concurrent::TimeoutException);
 task->run(); EXPECT_EQ(nullptr,task->get(0,::jxx::util::concurrent::TimeUnit::NANOSECONDS()));
}
TEST(FutureTaskParityTest, CancellationIsTerminalAndPreventsExecution){
 std::atomic<int> count{0}; auto task=::jxx::NEW<::jxx::util::concurrent::FutureTask<S>>(::jxx::NEW<Increment>(count),nullptr);
 EXPECT_TRUE(task->cancel(false)); EXPECT_TRUE(task->isCancelled()); EXPECT_TRUE(task->isDone());
 task->run(); EXPECT_EQ(0,count.load()); EXPECT_THROW(task->get(),::jxx::util::concurrent::CancellationException);
 EXPECT_FALSE(task->cancel(true));
}
}
