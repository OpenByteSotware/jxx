#include <gtest/gtest.h>
#include <atomic>

#include "lang/jxx.lang.InterruptedException.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.Thread.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"
#include "util/concurrent/jxx.util.concurrent.TimeUnit.h"

namespace {
using Task = ::jxx::util::concurrent::ScheduledFutureTask<::jxx::lang::Object>;

class NoOp final
    : public ::jxx::lang::ClassBase<NoOp,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    void run() override {}
};

class FutureWaiter final
    : public ::jxx::lang::ClassBase<FutureWaiter,
          ::jxx::lang::Object, ::jxx::lang::Runnable> {
public:
    FutureWaiter(const ::jxx::Ptr<Task>& task, ::jxx::lang::jbool timed)
        : task_(task), timed_(timed) {}

    void run() override {
        try {
            if (timed_) {
                (void)task_->get(30,
                    ::jxx::util::concurrent::TimeUnit::SECONDS());
            } else {
                (void)task_->get();
            }
        } catch (const ::jxx::lang::InterruptedException&) {
            interrupted_.store(true);
        }
    }

    ::jxx::lang::jbool interrupted() const noexcept {
        return interrupted_.load();
    }

private:
    ::jxx::Ptr<Task> task_;
    ::jxx::lang::jbool timed_;
    std::atomic<::jxx::lang::jbool> interrupted_{false};
};

void verifyInterruptedGet(::jxx::lang::jbool timed) {
    const auto task = ::jxx::NEW<Task>(
        ::jxx::CAST<::jxx::lang::Runnable>(::jxx::NEW<NoOp>()),
        nullptr,
        ::jxx::util::concurrent::TimeUnit::DAYS()->toNanos(1));
    const auto waiter = ::jxx::NEW<FutureWaiter>(task, timed);
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>(
        ::jxx::CAST<::jxx::lang::Runnable>(waiter));
    thread->start();
    ::jxx::lang::Thread::sleep(20);
    thread->interrupt();
    thread->join(2000);
    EXPECT_TRUE(waiter->interrupted());
    EXPECT_FALSE(thread->isAlive());
    EXPECT_FALSE(task->isDone());
}

TEST(ScheduledFutureGetParity, UntimedGetIsInterruptible) {
    verifyInterruptedGet(false);
}

TEST(ScheduledFutureGetParity, TimedGetIsInterruptible) {
    verifyInterruptedGet(true);
}

} // namespace
