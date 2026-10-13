#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"
#include "util/concurrent/jxx.util.concurrent.RejectedExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"

namespace {
class NullFactory final : public ::jxx::lang::ClassBase<NullFactory,
    ::jxx::lang::Object, ::jxx::util::concurrent::ThreadFactory> {
public:
    ::jxx::Ptr<::jxx::lang::Thread> newThread(
        const ::jxx::Ptr<::jxx::lang::Runnable>&) override { return nullptr; }
};
class NoOp final : public ::jxx::lang::ClassBase<NoOp,
    ::jxx::lang::Object, ::jxx::lang::Runnable> {
public: void run() override {}
};
TEST(ExecutorThreadFactoryFailureParity, NullWorkerRejectsInsteadOfStrandingTask) {
    using namespace ::jxx::util::concurrent;
    const auto executor = Executors::newFixedThreadPool(
        1, ::jxx::CAST<ThreadFactory>(::jxx::NEW<NullFactory>()));
    EXPECT_THROW(executor->execute(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>())),
        RejectedExecutionException);
    executor->shutdown();
    EXPECT_TRUE(executor->isTerminated());
}
}
