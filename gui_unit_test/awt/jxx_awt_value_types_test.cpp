#include <gtest/gtest.h>

#include "awt/jxx.awt.Dimension.h"
#include "awt/jxx.awt.Insets.h"
#include "awt/jxx.awt.Point.h"
#include "awt/jxx.awt.Rectangle.h"

TEST(AwtPointTest, MoveAndTranslateUpdateCoordinates)
{
    jxx::awt::Point point(4, 7);
    point.translate(-2, 5);
    EXPECT_EQ(point.x, 2);
    EXPECT_EQ(point.y, 12);
    point.move(9, 3);
    EXPECT_EQ(point.x, 9);
    EXPECT_EQ(point.y, 3);
}

TEST(AwtDimensionTest, SizeAccessorsPreserveValues)
{
    jxx::awt::Dimension value(320, 200);
    const auto copy = value.getSize();
    ASSERT_NE(copy, nullptr);
    EXPECT_EQ(copy->width, 320);
    EXPECT_EQ(copy->height, 200);
    value.setSize(640, 480);
    EXPECT_EQ(value.width, 640);
    EXPECT_EQ(value.height, 480);
}

TEST(AwtRectangleTest, ContainsIntersectsAndEmptyFollowBoundaryRules)
{
    auto other = jxx::NEW<jxx::awt::Rectangle>(8, 8, 5, 5);
    jxx::awt::Rectangle rectangle(2, 3, 10, 10);
    EXPECT_TRUE(rectangle.contains(2, 3));
    EXPECT_TRUE(rectangle.contains(11, 12));
    EXPECT_FALSE(rectangle.contains(12, 13));
    EXPECT_TRUE(rectangle.intersects(other));
    rectangle.setBounds(0, 0, 0, 10);
    EXPECT_TRUE(rectangle.isEmpty());
}

TEST(AwtInsetsTest, SetUpdatesAllEdges)
{
    jxx::awt::Insets insets(1, 2, 3, 4);
    insets.set(5, 6, 7, 8);
    EXPECT_EQ(insets.top, 5);
    EXPECT_EQ(insets.left, 6);
    EXPECT_EQ(insets.bottom, 7);
    EXPECT_EQ(insets.right, 8);
}
