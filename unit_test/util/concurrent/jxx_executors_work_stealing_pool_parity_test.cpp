#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.ForkJoinPool.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class Increment final : public ::jxx::lang::ClassBase<Increment,
    ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    explicit Increment(std::atomic<int>& value) : value_(value) {}
    void run() override { value_.fetch_add(1); }
private:
    std::atomic<int>& value_;
};

TEST(ExecutorsWorkStealingParity, ExplicitParallelismCreatesExecutorService) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newWorkStealingPool(2);
    ASSERT_NE(nullptr, service);
    const auto pool = ::jxx::CAST<ForkJoinPool>(service);
    ASSERT_NE(nullptr, pool);
    EXPECT_EQ(2, pool->getParallelism());
    std::atomic<int> value{0};
    const auto future = service->submit(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<Increment>(value)));
    (void)future->get();
    EXPECT_EQ(1, value.load());
    service->shutdown();
    EXPECT_TRUE(service->awaitTermination(2, TimeUnit::SECONDS()));
}

TEST(ExecutorsWorkStealingParity, NonPositiveParallelismIsRejected) {
    using namespace ::jxx::util::concurrent;
    EXPECT_THROW(Executors::newWorkStealingPool(0),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(Executors::newWorkStealingPool(-1),
                 ::jxx::lang::IllegalArgumentException);
}

TEST(ExecutorsWorkStealingParity, NoArgFactoryUsesPositiveParallelism) {
    using namespace ::jxx::util::concurrent;
    const auto service = Executors::newWorkStealingPool();
    const auto pool = ::jxx::CAST<ForkJoinPool>(service);
    ASSERT_NE(nullptr, pool);
    EXPECT_GT(pool->getParallelism(), 0);
    service->shutdownNow();
}
} // namespace
