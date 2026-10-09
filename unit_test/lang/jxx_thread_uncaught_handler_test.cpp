#include <gtest/gtest.h>
#include <atomic>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Runnable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Throwable.h"

namespace {
using ::jxx::lang::Object;
using ::jxx::lang::Runnable;
using ::jxx::lang::String;
using ::jxx::lang::Thread;
using ::jxx::lang::Throwable;

class ThrowingRunnable final
    : public ::jxx::lang::ClassBase<ThrowingRunnable, Object, Runnable> {
public:
    void run() override {
        throw ::jxx::lang::IllegalStateException(
            ::jxx::NEW<String>("uncaught"));
    }
};

class RecordingHandler final
    : public ::jxx::lang::ClassBase<
          RecordingHandler, Object, Thread::UncaughtExceptionHandler> {
public:
    void uncaughtException(
        const ::jxx::Ptr<Thread>& thread,
        const ::jxx::Ptr<Throwable>& throwable) override {
        threadSeen_.store(thread != nullptr);
        throwableSeen_.store(throwable != nullptr);
        calls_.fetch_add(1);
    }

    int calls() const { return calls_.load(); }
    bool threadSeen() const { return threadSeen_.load(); }
    bool throwableSeen() const { return throwableSeen_.load(); }

private:
    std::atomic<int> calls_{0};
    std::atomic<bool> threadSeen_{false};
    std::atomic<bool> throwableSeen_{false};
};

TEST(ThreadUncaughtHandlerParity, ExplicitHandlerReceivesThrowable) {
    const auto handler = ::jxx::NEW<RecordingHandler>();
    const auto thread = ::jxx::NEW<Thread>(
        ::jxx::CAST<Runnable>(::jxx::NEW<ThrowingRunnable>()));
    thread->setUncaughtExceptionHandler(
        ::jxx::CAST<Thread::UncaughtExceptionHandler>(handler));

    thread->start();
    thread->join();

    EXPECT_EQ(1, handler->calls());
    EXPECT_TRUE(handler->threadSeen());
    EXPECT_TRUE(handler->throwableSeen());
}
} // namespace
