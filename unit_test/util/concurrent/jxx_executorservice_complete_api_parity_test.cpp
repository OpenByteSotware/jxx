#include <gtest/gtest.h>
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/jxx.util.ArrayList.h"
namespace {
class ReturnObject final : public ::jxx::lang::ClassBase<ReturnObject, ::jxx::lang::Object, ::jxx::util::concurrent::Callable<::jxx::lang::Object>> {
public:
 explicit ReturnObject(const ::jxx::Ptr<::jxx::lang::Object>& v) : v_(v) {}
 ::jxx::Ptr<::jxx::lang::Object> call() override { return v_; }
private: ::jxx::Ptr<::jxx::lang::Object> v_;
};
TEST(ExecutorServiceParity, CompleteGenericApisAreAvailableThroughInterface) {
 using namespace ::jxx::util::concurrent;
 const ::jxx::Ptr<ExecutorService> service = Executors::newFixedThreadPool(2);
 const auto value = ::jxx::NEW<::jxx::lang::Object>();
 const auto callable = ::jxx::CAST<Callable<::jxx::lang::Object>>(::jxx::NEW<ReturnObject>(value));
 EXPECT_EQ(value, service->submit<::jxx::lang::Object>(callable)->get());
 auto tasks = ::jxx::NEW<::jxx::util::ArrayList<Callable<::jxx::lang::Object>>>();
 tasks->add(callable);
 const auto list = ::jxx::CAST<::jxx::util::List<Callable<::jxx::lang::Object>>>(tasks);
 EXPECT_EQ(1, service->invokeAll<::jxx::lang::Object>(list)->size());
 EXPECT_EQ(value, service->invokeAny<::jxx::lang::Object>(list));
 service->shutdown();
}
} // namespace
