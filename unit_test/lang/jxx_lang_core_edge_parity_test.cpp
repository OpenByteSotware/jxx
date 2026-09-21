#include <gtest/gtest.h>

#include "lang/jxx.lang.Compiler.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"

namespace {
using ::jxx::lang::Compiler;
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::StackTraceElement;
using ::jxx::lang::String;
using ::jxx::lang::Thread;
using ::jxx::lang::Throwable;
using ::jxx::lang::UnsupportedOperationException;

TEST(ThreadDeprecatedParityTest, UnsafeOperationsHaveDeterministicPortableBehavior) {
    const auto thread = ::jxx::NEW<Thread>();
    EXPECT_THROW(thread->stop(), UnsupportedOperationException);
    EXPECT_THROW(thread->suspend(), UnsupportedOperationException);
    EXPECT_THROW(thread->resume(), UnsupportedOperationException);
    EXPECT_THROW(thread->destroy(), UnsupportedOperationException);
    EXPECT_EQ(0, thread->countStackFrames());
}

TEST(ThreadDeprecatedParityTest, HoldsLockValidatesNullBeforeUnsupportedOperation) {
    EXPECT_THROW(Thread::holdsLock(nullptr), NullPointerException);
    EXPECT_THROW(Thread::holdsLock(::jxx::NEW<::jxx::lang::Object>()),
                 UnsupportedOperationException);
}

TEST(ThrowableParityTest, SuppressionRejectsNullAndSelfAndReturnsDefensiveArray) {
    const auto value = ::jxx::NEW<Throwable>("root");
    EXPECT_THROW(value->addSuppressed(nullptr), NullPointerException);
    EXPECT_THROW(value->addSuppressed(value), IllegalArgumentException);
    value->addSuppressed(::jxx::NEW<Throwable>("child"));
    const auto first = value->getSuppressed();
    const auto second = value->getSuppressed();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(first.get(), second.get());
    EXPECT_EQ(1U, first->length);
}

TEST(ThrowableParityTest, StackTraceSetterRejectsNullElementsAndReturnsDefensiveArray) {
    const auto value = ::jxx::NEW<Throwable>("root");
    auto trace = ::jxx::NEW<Throwable::StackTraceArrayType>(1U);
    (*trace)[0] = nullptr;
    EXPECT_THROW(value->setStackTrace(trace), NullPointerException);

    (*trace)[0] = ::jxx::NEW<StackTraceElement>(
        ::jxx::NEW<String>("Example"), ::jxx::NEW<String>("run"),
        ::jxx::NEW<String>("Example.cpp"), 42);
    value->setStackTrace(trace);
    EXPECT_NE(value->getStackTrace().get(), value->getStackTrace().get());
}

TEST(StackTraceElementParityTest, FormatsNativeFileLineAndUnknownSourceCases) {
    const auto type = ::jxx::NEW<String>("Example");
    const auto method = ::jxx::NEW<String>("run");
    EXPECT_EQ("Example.run(Native Method)",
              StackTraceElement(type, method, nullptr, -2).toString()->utf8());
    EXPECT_EQ("Example.run(Example.cpp:42)",
              StackTraceElement(type, method, ::jxx::NEW<String>("Example.cpp"), 42)
                  .toString()->utf8());
    EXPECT_EQ("Example.run(Unknown Source)",
              StackTraceElement(type, method, nullptr, -1).toString()->utf8());
}

TEST(CompilerParityTest, NoCompilerServiceReturnsDocumentedSentinels) {
    EXPECT_THROW(Compiler::compileClass(nullptr), NullPointerException);
    EXPECT_THROW(Compiler::compileClasses(nullptr), NullPointerException);
    EXPECT_THROW(Compiler::command(nullptr), NullPointerException);
}
} // namespace
