#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"
#include "util/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.concurrent.AbstractExecutorService.h"
#include "util/jxx.util.concurrent.Executors.h"
#include "util/jxx.util.concurrent.TimeUnit.h"
#include "util/jxx.util.concurrent.TimeoutException.h"
namespace {
using S=::jxx::lang::String;
class Value final:public ::jxx::util::concurrent::Callable<S>{public:explicit Value(const char* v):v_(v){} ::jxx::Ptr<S> call()override{return ::jxx::NEW<S>(v_);}private:std::string v_;};
class NoOp final:public ::jxx::lang::Runnable{public:void run()override{}};
TEST(ExecutorSubmitParityTest, CallableAndRunnableResultOverloadsComplete){
 auto e=::jxx::util::concurrent::Executors::newFixedThreadPool(2); auto a=::jxx::CAST<::jxx::util::concurrent::AbstractExecutorService>(e);
 auto c=::jxx::CAST<::jxx::util::concurrent::Callable<S>>(::jxx::NEW<Value>("c")); EXPECT_TRUE(a->submit<S>(c)->get()->utf8()=="c");
 auto r=::jxx::NEW<S>("r"); EXPECT_EQ(r.get(),a->submit<S>(::jxx::NEW<NoOp>(),r)->get().get()); e->shutdown();
}
TEST(ExecutorBulkParityTest, InvokeAllAndInvokeAnyReturnResults){
 auto e=::jxx::util::concurrent::Executors::newFixedThreadPool(2); auto a=::jxx::CAST<::jxx::util::concurrent::AbstractExecutorService>(e);
 auto tasks=::jxx::NEW<::jxx::util::ArrayList<::jxx::util::concurrent::Callable<S>>>(); tasks->add(::jxx::CAST<::jxx::util::concurrent::Callable<S>>(::jxx::NEW<Value>("a"))); tasks->add(::jxx::CAST<::jxx::util::concurrent::Callable<S>>(::jxx::NEW<Value>("b")));
 auto list=::jxx::CAST<::jxx::util::List<::jxx::util::concurrent::Callable<S>>>(tasks); auto futures=a->invokeAll<S>(list); EXPECT_EQ(2,futures->size()); ASSERT_NE(nullptr,a->invokeAny<S>(list)); e->shutdown();
}
}
