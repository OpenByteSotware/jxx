#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Thread.h"
#include "util/jxx.util.EnumMap.h"

TEST(EnumMapParity, IteratesEntriesInDeclarationOrder) {
    using State = ::jxx::lang::Thread::State;
    const auto map = ::jxx::NEW<::jxx::util::EnumMap<State, ::jxx::lang::String>>(
        State::Class());
    map->put(State::WAITING, ::jxx::NEW<::jxx::lang::String>("waiting"));
    map->put(State::NEW, ::jxx::NEW<::jxx::lang::String>("new"));
    map->put(State::RUNNABLE, ::jxx::NEW<::jxx::lang::String>("runnable"));

    const auto iteratorValue = map->entrySet()->iterator();
    EXPECT_EQ(State::NEW.get(), iteratorValue->next()->getKey().get());
    EXPECT_EQ(State::RUNNABLE.get(), iteratorValue->next()->getKey().get());
    EXPECT_EQ(State::WAITING.get(), iteratorValue->next()->getKey().get());
}

TEST(EnumMapParity, SupportsNullValuesAndDistinguishesPresence) {
    using State = ::jxx::lang::Thread::State;
    const auto map = ::jxx::NEW<::jxx::util::EnumMap<State, ::jxx::lang::String>>(
        State::Class());
    EXPECT_EQ(nullptr, map->put(State::NEW, nullptr));
    EXPECT_TRUE(map->containsKey(
        ::jxx::CAST<::jxx::lang::Object>(State::NEW)));
    EXPECT_EQ(nullptr, map->get(
        ::jxx::CAST<::jxx::lang::Object>(State::NEW)));
    EXPECT_EQ(1, map->size());
}

TEST(EnumMapParity, RejectsNullKeys) {
    using State = ::jxx::lang::Thread::State;
    const auto map = ::jxx::NEW<::jxx::util::EnumMap<State, ::jxx::lang::String>>(
        State::Class());
    EXPECT_THROW(map->put(nullptr, nullptr), ::jxx::lang::NullPointerException);
}
