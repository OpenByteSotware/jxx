#include <gtest/gtest.h>

#include <limits>

#include "awt/jxx.awt.Rectangle.h"

TEST(RectangleParity, ContainsRectangle)
{
    auto outer = ::jxx::NEW<::jxx::awt::Rectangle>(0, 0, 10, 10);
    EXPECT_TRUE(outer->contains(::jxx::NEW<::jxx::awt::Rectangle>(0, 0, 10, 10)));
    EXPECT_TRUE(outer->contains(::jxx::NEW<::jxx::awt::Rectangle>(2, 3, 4, 5)));
    EXPECT_TRUE(outer->contains(::jxx::NEW<::jxx::awt::Rectangle>(10, 10, 0, 0)));
    EXPECT_FALSE(outer->contains(::jxx::NEW<::jxx::awt::Rectangle>(9, 9, 2, 2)));
    EXPECT_FALSE(outer->contains(nullptr));
}

TEST(RectangleParity, ContainsRectangleHandlesInvalidAndOverflowingBounds)
{
    auto outer = ::jxx::NEW<::jxx::awt::Rectangle>(0, 0, 10, 10);
    EXPECT_FALSE(outer->contains(::jxx::NEW<::jxx::awt::Rectangle>(1, 1, -1, 1)));
    auto large = ::jxx::NEW<::jxx::awt::Rectangle>(
        std::numeric_limits<::jxx::lang::jint>::max() - 5, 0, 5, 5);
    EXPECT_TRUE(large->contains(::jxx::NEW<::jxx::awt::Rectangle>(
        std::numeric_limits<::jxx::lang::jint>::max() - 1, 1, 1, 1)));
}
