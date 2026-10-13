#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "security/jxx.security.PrivilegedAction.h"
#include "security/jxx.security.PrivilegedExceptionAction.h"
#include "util/concurrent/jxx.util.concurrent.Executors.h"

namespace {

class TestAction final
    : public ::jxx::lang::ClassBase<
          TestAction,
          ::jxx::lang::Object,
          ::jxx::security::PrivilegedAction<::jxx::lang::Integer>> {
public:
    ::jxx::Ptr<::jxx::lang::Integer> run() override {
        return ::jxx::lang::Integer::valueOf(17);
    }
};

class TestExceptionAction final
    : public ::jxx::lang::ClassBase<
          TestExceptionAction,
          ::jxx::lang::Object,
          ::jxx::security::PrivilegedExceptionAction<::jxx::lang::Integer>> {
public:
    ::jxx::Ptr<::jxx::lang::Integer> run() override {
        return ::jxx::lang::Integer::valueOf(23);
    }
};

TEST(ExecutorsPrivilegedApiTest, CallableRunsPrivilegedAction) {
    auto action = ::jxx::NEW<TestAction>();
    auto callable = ::jxx::util::concurrent::Executors::callable<
        ::jxx::lang::Integer>(action);
    EXPECT_EQ(17, callable->call()->intValue());
}

TEST(ExecutorsPrivilegedApiTest, CallableRunsPrivilegedExceptionAction) {
    auto action = ::jxx::NEW<TestExceptionAction>();
    auto callable = ::jxx::util::concurrent::Executors::callable<
        ::jxx::lang::Integer>(action);
    EXPECT_EQ(23, callable->call()->intValue());
}

TEST(ExecutorsPrivilegedApiTest, PrivilegedAdaptersRejectNull) {
    ::jxx::Ptr<::jxx::util::concurrent::Callable<::jxx::lang::Integer>> task;
    EXPECT_THROW(
        ::jxx::util::concurrent::Executors::privilegedCallable<
            ::jxx::lang::Integer>(task),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::util::concurrent::Executors::privilegedCallableUsingCurrentClassLoader<
            ::jxx::lang::Integer>(task),
        ::jxx::lang::NullPointerException);
}

TEST(ExecutorsPrivilegedApiTest, PrivilegedThreadFactoryCreatesThread) {
    auto factory = ::jxx::util::concurrent::Executors::privilegedThreadFactory();
    ASSERT_NE(nullptr, factory);
}

} // namespace
