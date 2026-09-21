#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.Runnable.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.ScheduledExecutorService.h"
#include "util/jxx.util.concurrent.ScheduledFuture.h"
#include "util/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
namespace {
class Increment final:public ::jxx::lang::Runnable{public:explicit Increment(std::atomic<int>& v):v_(v){}void run()override{++v_;}private:std::atomic<int>& v_;};
TEST(ScheduledExecutorShutdownPolicyTest, DefaultPoliciesMatchExpectedLifecycle){
 auto concrete=::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
 EXPECT_FALSE(concrete->getContinueExistingPeriodicTasksAfterShutdownPolicy());
 EXPECT_TRUE(concrete->getExecuteExistingDelayedTasksAfterShutdownPolicy());
 concrete->shutdown(); EXPECT_TRUE(concrete->isShutdown());
 EXPECT_TRUE(concrete->awaitTermination(2,::jxx::util::concurrent::TimeUnit::SECONDS()));
}
TEST(ScheduledExecutorShutdownPolicyTest, DelayedTaskExecutesAfterOrderlyShutdownByDefault){
 std::atomic<int> count{0}; auto concrete=::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
 auto service=::jxx::CAST<::jxx::util::concurrent::ScheduledExecutorService>(concrete);
 auto future=service->schedule(::jxx::NEW<Increment>(count),1,::jxx::util::concurrent::TimeUnit::MILLISECONDS());
 concrete->shutdown(); (void)future->get(); EXPECT_EQ(1,count.load());
 EXPECT_TRUE(concrete->awaitTermination(2,::jxx::util::concurrent::TimeUnit::SECONDS()));
}
TEST(ScheduledExecutorShutdownPolicyTest, DisabledDelayedPolicyCancelsWaitingFuture){
 std::atomic<int> count{0}; auto concrete=::jxx::NEW<::jxx::util::concurrent::ScheduledThreadPoolExecutor>(1);
 concrete->setExecuteExistingDelayedTasksAfterShutdownPolicy(false);
 auto future=concrete->schedule(::jxx::NEW<Increment>(count),1,::jxx::util::concurrent::TimeUnit::SECONDS());
 concrete->shutdown(); EXPECT_TRUE(future->isCancelled()); EXPECT_EQ(0,count.load());
 EXPECT_TRUE(concrete->awaitTermination(2,::jxx::util::concurrent::TimeUnit::SECONDS()));
}
}
