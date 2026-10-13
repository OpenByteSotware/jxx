#include <gtest/gtest.h>
#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.FutureTask.h"
#include "util/concurrent/jxx.util.concurrent.ThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"
namespace {
class NoOp final : public ::jxx::lang::ClassBase<NoOp, ::jxx::lang::Object, ::jxx::lang::Runnable> {
public: void run() override {}
};
TEST(ThreadPoolPurgeParity, RemovesCancelledFutureTasksFromQueue) {
    using namespace ::jxx::util::concurrent;
    const auto executor = ::jxx::NEW<ThreadPoolExecutor>(0, 1, 60, TimeUnit::SECONDS());
    const auto future = ::jxx::NEW<FutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()), nullptr);
    executor->execute(::jxx::CAST<::jxx::lang::Runnable>(future));
    EXPECT_TRUE(future->cancel(false));
    executor->purge();
    const auto pending = executor->shutdownNow();
    ASSERT_NE(nullptr, pending);
    EXPECT_EQ(0, pending->size());
}
}
