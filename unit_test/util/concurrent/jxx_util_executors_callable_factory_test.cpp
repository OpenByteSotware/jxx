#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.concurrent.Executors.h"

namespace {
class Increment final : public ::jxx::lang::Runnable {
public: explicit Increment(std::atomic<int>& value):value_(value){} void run() override{++value_;}
private: std::atomic<int>& value_;
};
TEST(ExecutorsCallableParityTest, RunnableAdaptersReturnConfiguredResults) {
 std::atomic<int> count{0}; const auto runnable=::jxx::NEW<Increment>(count);
 const auto empty=::jxx::util::concurrent::Executors::callable(runnable);
 EXPECT_EQ(nullptr,empty->call()); EXPECT_EQ(1,count.load());
 const auto result=::jxx::NEW<::jxx::lang::String>("done");
 const auto valued=::jxx::util::concurrent::Executors::callable(runnable,result);
 EXPECT_EQ(result.get(),valued->call().get()); EXPECT_EQ(2,count.load());
}
TEST(ExecutorsThreadFactoryParityTest, DefaultFactoryCreatesNormalizedThreads) {
 std::atomic<int> count{0}; const auto factory=::jxx::util::concurrent::Executors::defaultThreadFactory();
 const auto thread=factory->newThread(::jxx::NEW<Increment>(count)); ASSERT_NE(nullptr,thread);
 EXPECT_FALSE(thread->isDaemon()); EXPECT_EQ(::jxx::lang::Thread::NORM_PRIORITY,thread->getPriority());
 thread->start(); thread->join(); EXPECT_EQ(1,count.load());
}
}
