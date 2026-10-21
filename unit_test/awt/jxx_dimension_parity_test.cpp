#include <gtest/gtest.h>

#include "awt/geom/jxx.awt.geom.Dimension2D.h"
#include "awt/jxx.awt.Dimension.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"

TEST(DimensionParityTest, ExposesDimension2DHierarchy)
{
    const auto value = ::jxx::NEW<::jxx::awt::Dimension>(12, 34);
    EXPECT_NE(::jxx::CAST<::jxx::awt::geom::Dimension2D>(value), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::lang::Cloneable>(value), nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::io::SerializableI>(value), nullptr);
}

TEST(DimensionParityTest, ReportsDoublePrecisionAccessors)
{
    const auto value = ::jxx::NEW<::jxx::awt::Dimension>(12, 34);
    EXPECT_DOUBLE_EQ(value->getWidth(), 12.0);
    EXPECT_DOUBLE_EQ(value->getHeight(), 34.0);
}

TEST(DimensionParityTest, DoubleSetSizeRoundsTowardPositiveInfinity)
{
    const auto value = ::jxx::NEW<::jxx::awt::Dimension>();
    value->setSize(12.01, 34.01);
    EXPECT_EQ(value->width, 13);
    EXPECT_EQ(value->height, 35);

    value->setSize(-12.99, -34.99);
    EXPECT_EQ(value->width, -12);
    EXPECT_EQ(value->height, -34);
}

TEST(DimensionParityTest, GetSizeReturnsIndependentCopy)
{
    const auto value = ::jxx::NEW<::jxx::awt::Dimension>(12, 34);
    const auto copy = value->getSize();
    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy, value);
    EXPECT_TRUE(value->equals(::jxx::CAST<::jxx::lang::Object>(copy)));
}

TEST(DimensionParityTest, SerializationRoundTripPreservesFields)
{
    const auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    const auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<::jxx::awt::Dimension>(12, 34)));
    output->flush();

    const auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    const auto restored = ::jxx::CAST<::jxx::awt::Dimension>(
        input->readObject());

    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->width, 12);
    EXPECT_EQ(restored->height, 34);
}
