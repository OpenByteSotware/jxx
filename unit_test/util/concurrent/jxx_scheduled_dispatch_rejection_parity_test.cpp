#include <gtest/gtest.h>

#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledThreadPoolExecutor.h"
#include "util/concurrent/jxx.util.concurrent.ThreadFactory.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
class NullFactory final
    : public ::jxx::lang::ClassBase<NullFactory,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::ThreadFactory> {
public:
    ::jxx::Ptr<::jxx::lang::Thread> newThread(
        const ::jxx::Ptr<::jxx::lang::Runnable>&) override {
        return nullptr;
    }
};

class NoOp final
    : public ::jxx::lang::ClassBase<NoOp,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {}
};

TEST(ScheduledDispatchRejectionParity,
     AcceptedTaskBecomesCancelledWhenWorkerCannotBeCreated) {
    using namespace ::jxx::util::concurrent;
    const auto executor = ::jxx::NEW<ScheduledThreadPoolExecutor>(
        1, ::jxx::CAST<ThreadFactory>(::jxx::NEW<NullFactory>()));
    const auto future = executor->schedule(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()),
        0, TimeUnit::NANOSECONDS());

    for (::jxx::lang::jint index = 0;
         index < 200 && !future->isDone(); ++index) {
        ::jxx::lang::Thread::sleep(1);
    }

    EXPECT_TRUE(future->isDone());
    EXPECT_TRUE(future->isCancelled());
    executor->shutdownNow();
}
} // namespace
