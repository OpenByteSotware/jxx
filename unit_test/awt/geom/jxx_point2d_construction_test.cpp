#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"

TEST(Point2DConstructionParity, DoubleCoordinatesConstructThroughJxxNew)
{
    ::jxx::initialize();
    auto point = ::jxx::NEW<::jxx::awt::geom::Point2D::Double>(1.25, -2.5);
    ASSERT_NE(point, nullptr);
    EXPECT_DOUBLE_EQ(point->getX(), 1.25);
    EXPECT_DOUBLE_EQ(point->getY(), -2.5);
}

TEST(Point2DConstructionParity, FloatCoordinatesConstructThroughJxxNew)
{
    auto point = ::jxx::NEW<::jxx::awt::geom::Point2D::Float>(1.25F, -2.5F);
    ASSERT_NE(point, nullptr);
    EXPECT_DOUBLE_EQ(point->getX(), 1.25);
    EXPECT_DOUBLE_EQ(point->getY(), -2.5);
}

TEST(Point2DConstructionParity, DefaultConstructorsInitializeOrigin)
{
    auto doublePoint = ::jxx::NEW<::jxx::awt::geom::Point2D::Double>();
    auto floatPoint = ::jxx::NEW<::jxx::awt::geom::Point2D::Float>();
    EXPECT_DOUBLE_EQ(doublePoint->getX(), 0.0);
    EXPECT_DOUBLE_EQ(doublePoint->getY(), 0.0);
    EXPECT_DOUBLE_EQ(floatPoint->getX(), 0.0);
    EXPECT_DOUBLE_EQ(floatPoint->getY(), 0.0);
}
