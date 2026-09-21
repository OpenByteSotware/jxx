#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.TimeUnit.h"

namespace {
class ReturnValue final : public ::jxx::util::concurrent::Callable<::jxx::lang::String> {
public: ::jxx::Ptr<::jxx::lang::String> call() override {
 return ::jxx::NEW<::jxx::lang::String>("scheduled-result");
}
};
TEST(ScheduledCallableParityTest, CallableResultCompletesThroughScheduledFuture) {
 const auto executor=::jxx::util::concurrent::Executors::newSingleThreadScheduledExecutor();
 const auto callable = ::jxx::CAST<
  ::jxx::util::concurrent::Callable<::jxx::lang::String>>(
      ::jxx::NEW<ReturnValue>());
 const auto future=executor->schedule(
  callable,0,::jxx::util::concurrent::TimeUnit::MILLISECONDS());
 ASSERT_NE(nullptr,future);
 const auto result=future->get(); ASSERT_NE(nullptr,result);
 EXPECT_TRUE(result->utf8()=="scheduled-result"); EXPECT_TRUE(future->isDone());
 executor->shutdown();
}
}
