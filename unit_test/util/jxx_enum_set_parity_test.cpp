#include <gtest/gtest.h>

#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.EnumSet.h"

TEST(EnumSetParity, IteratesInDeclarationOrder) {
    using State = ::jxx::lang::Thread::State;
    const auto values = ::jxx::util::EnumSet<State>::range(
        State::RUNNABLE, State::WAITING);
    ASSERT_EQ(3, values->size());
    const auto iteratorValue = values->iterator();
    EXPECT_EQ(State::RUNNABLE.get(), iteratorValue->next().get());
    EXPECT_EQ(State::BLOCKED.get(), iteratorValue->next().get());
    EXPECT_EQ(State::WAITING.get(), iteratorValue->next().get());
}

TEST(EnumSetParity, ComplementUsesSameEnumUniverse) {
    using State = ::jxx::lang::Thread::State;
    const auto selected = ::jxx::util::EnumSet<State>::of(
        State::NEW, State::TERMINATED);
    const auto complement =
        ::jxx::util::EnumSet<State>::complementOf(selected);
    EXPECT_EQ(4, complement->size());
    EXPECT_FALSE(complement->contains(
        ::jxx::CAST<::jxx::lang::Object>(State::NEW)));
    EXPECT_TRUE(complement->contains(
        ::jxx::CAST<::jxx::lang::Object>(State::RUNNABLE)));
}

TEST(EnumSetParity, RejectsNullAndReversedRange) {
    using State = ::jxx::lang::Thread::State;
    EXPECT_THROW(
        ::jxx::util::EnumSet<State>::of(nullptr),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::util::EnumSet<State>::range(State::WAITING, State::RUNNABLE),
        ::jxx::lang::IllegalArgumentException);
}
