#include <gtest/gtest.h>

#include "lang/jxx.lang.StackTraceElement.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"
#include "lang/jxx.lang.buildin_array.h"

TEST(JxxLangStackTraceElementStage, ExposesFrameProperties) {
    const auto frame = ::jxx::NEW<::jxx::lang::StackTraceElement>(
        ::jxx::NEW<::jxx::lang::String>("example.Type"),
        ::jxx::NEW<::jxx::lang::String>("call"),
        ::jxx::NEW<::jxx::lang::String>("Type.cpp"),
        42);
    EXPECT_EQ(frame->getClassName()->utf8(), "example.Type");
    EXPECT_EQ(frame->getMethodName()->utf8(), "call");
    EXPECT_EQ(frame->getFileName()->utf8(), "Type.cpp");
    EXPECT_EQ(frame->getLineNumber(), 42);
    EXPECT_FALSE(frame->isNativeMethod());
    EXPECT_EQ(frame->toString()->utf8(), "example.Type.call(Type.cpp:42)");
}

TEST(JxxLangStackTraceElementStage, ThrowableUsesDefensiveJxxArray) {
    auto throwable = ::jxx::NEW<::jxx::lang::Throwable>("failure");
    auto trace = ::jxx::NEW<::jxx::lang::Throwable::StackTraceArrayType>(1);
    trace->operator[](0) = ::jxx::NEW<::jxx::lang::StackTraceElement>(
        ::jxx::NEW<::jxx::lang::String>("example.Type"),
        ::jxx::NEW<::jxx::lang::String>("call"),
        nullptr,
        -1);
    throwable->setStackTrace(trace);
    const auto first = throwable->getStackTrace();
    const auto second = throwable->getStackTrace();
    ASSERT_NE(first, second);
    EXPECT_EQ(first->length, 1U);
    first->operator[](0) = nullptr;
    EXPECT_NE(second->operator[](0), nullptr);
}
