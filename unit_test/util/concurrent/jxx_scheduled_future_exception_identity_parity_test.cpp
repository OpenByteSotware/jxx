#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.Callable.h"
#include "util/concurrent/jxx.util.concurrent.ExecutionException.h"
#include "util/concurrent/jxx.util.concurrent.ScheduledFutureTask.h"

namespace {

class ThrowingCallable final
    : public ::jxx::lang::ClassBase<
          ThrowingCallable,
          ::jxx::lang::Object,
          ::jxx::util::concurrent::Callable<::jxx::lang::Object>> {
public:
    ::jxx::Ptr<::jxx::lang::Object> call() override {
        throw ::jxx::lang::IllegalStateException(
            ::jxx::NEW<::jxx::lang::String>("scheduled failure"));
    }
};

TEST(ScheduledFutureExceptionParity, PreservesConcreteThrowableAsExecutionCause) {
    using namespace ::jxx::util::concurrent;
    const auto task = ::jxx::NEW<ScheduledFutureTask<::jxx::lang::Object>>(
        ::jxx::CAST<Callable<::jxx::lang::Object>>(
            ::jxx::NEW<ThrowingCallable>()),
        0);

    task->run();
    ASSERT_TRUE(task->isDone());

    try {
        (void)task->get();
        FAIL() << "Expected ExecutionException";
    } catch (const ExecutionException& failure) {
        const auto cause = failure.getCause();
        ASSERT_NE(nullptr, cause);
        EXPECT_NE(nullptr,
            ::jxx::CAST<::jxx::lang::IllegalStateException>(cause));
        EXPECT_EQ("scheduled failure", cause->getMessage()->utf8());
    }
}

} // namespace
