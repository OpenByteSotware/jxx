#include <gtest/gtest.h>

#include "lang/jxx.lang.ClassInitialization.h"
#include "lang/jxx.lang.Thread.h"

TEST(LangClassInitializationParity, RegistersThreadUncaughtExceptionHandler)
{
    EXPECT_NO_THROW(::jxx::lang::initializeClasses());
    EXPECT_NE(nullptr, ::jxx::lang::Thread::UncaughtExceptionHandler::Class());
}

TEST(LangClassInitializationParity, RemainsIdempotent)
{
    EXPECT_NO_THROW(::jxx::lang::initializeClasses());
    EXPECT_NO_THROW(::jxx::lang::initializeClasses());
}
