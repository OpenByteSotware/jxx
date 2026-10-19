#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

TEST(Point2DClassInitialization, NestedConcreteClassesHaveDistinctMetadata)
{
    ::jxx::initialize();
    auto floatClass = ::jxx::awt::geom::Point2D::Float::Class();
    auto doubleClass = ::jxx::awt::geom::Point2D::Double::Class();
    ASSERT_NE(floatClass, nullptr);
    ASSERT_NE(doubleClass, nullptr);
    EXPECT_NE(floatClass, doubleClass);
    EXPECT_EQ(floatClass->getName()->utf8(), "jxx.awt.geom.Point2D.Float");
    EXPECT_EQ(doubleClass->getName()->utf8(), "jxx.awt.geom.Point2D.Double");
}

TEST(Point2DSerialization, DoubleRoundTripPreservesCoordinates)
{
    ::jxx::initialize();
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::NEW<::jxx::awt::geom::Point2D::Double>(1.25, -2.5));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<::jxx::awt::geom::Point2D::Double>(input->readObject());
    ASSERT_NE(restored, nullptr);
    EXPECT_DOUBLE_EQ(restored->getX(), 1.25);
    EXPECT_DOUBLE_EQ(restored->getY(), -2.5);
}

TEST(Point2DSerialization, FloatRoundTripPreservesCoordinates)
{
    ::jxx::initialize();
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::NEW<::jxx::awt::geom::Point2D::Float>(3.5F, -7.25F));
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    auto restored = ::jxx::CAST<::jxx::awt::geom::Point2D::Float>(input->readObject());
    ASSERT_NE(restored, nullptr);
    EXPECT_FLOAT_EQ(restored->x, 3.5F);
    EXPECT_FLOAT_EQ(restored->y, -7.25F);
}
