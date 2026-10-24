#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "util/jxx.util.IntStream.h"

TEST(IntStreamInitializationTest, GlobalInitializationMakesClassAvailable) {
    ::jxx::initialize();
    EXPECT_NE(nullptr, ::jxx::util::IntStream::Class());
}

TEST(IntStreamInitializationTest, InitializationIsIdempotent) {
    ::jxx::initialize();
    auto first = ::jxx::util::IntStream::Class();
    ::jxx::initialize();
    auto second = ::jxx::util::IntStream::Class();
    EXPECT_EQ(first, second);
}
