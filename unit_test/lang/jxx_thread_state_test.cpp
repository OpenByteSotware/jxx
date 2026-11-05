#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"

TEST(ThreadStateTest, ExposesEnumObjectSemantics) {
    const auto values = ::jxx::lang::Thread::State::values();
    ASSERT_NE(nullptr, values);
    ASSERT_EQ(6, values->length);

    EXPECT_EQ(::jxx::lang::Thread::State::NEW, (*values)[0]);
    EXPECT_EQ(::jxx::lang::Thread::State::RUNNABLE, (*values)[1]);
    EXPECT_EQ(::jxx::lang::Thread::State::BLOCKED, (*values)[2]);
    EXPECT_EQ(::jxx::lang::Thread::State::WAITING, (*values)[3]);
    EXPECT_EQ(::jxx::lang::Thread::State::TIMED_WAITING, (*values)[4]);
    EXPECT_EQ(::jxx::lang::Thread::State::TERMINATED, (*values)[5]);

    EXPECT_EQ(0, ::jxx::lang::Thread::State::NEW->ordinal());
    EXPECT_EQ(5, ::jxx::lang::Thread::State::TERMINATED->ordinal());
    EXPECT_TRUE(::jxx::lang::Thread::State::RUNNABLE->name()->equals(
        ::jxx::NEW<::jxx::lang::String>("RUNNABLE")));
}

TEST(ThreadStateTest, ValueOfReturnsCanonicalConstants) {
    EXPECT_EQ(
        ::jxx::lang::Thread::State::WAITING,
        ::jxx::lang::Thread::State::valueOf(
            ::jxx::NEW<::jxx::lang::String>("WAITING")));
}

TEST(ThreadStateTest, ThreadReturnsStateObject) {
    const auto thread = ::jxx::NEW<::jxx::lang::Thread>();
    EXPECT_EQ(::jxx::lang::Thread::State::NEW, thread->getState());
}
