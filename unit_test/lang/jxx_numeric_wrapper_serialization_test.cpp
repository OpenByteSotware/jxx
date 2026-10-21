#include <cmath>
#include <limits>

#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.JxxArray.h"

namespace {

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }
    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}

template <typename Wrapper, typename Primitive>
void expectIntegralRoundTrip(Primitive value) {
    auto restored = roundTrip(::jxx::NEW<Wrapper>(value));
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(value), restored->longValue());
}

template <typename Wrapper, typename Primitive>
void expectRankOneArrayRoundTrip(Primitive first, Primitive second) {
    using Array = ::jxx::lang::JxxArray<::jxx::Ptr<Wrapper>, 1U>;
    auto original = ::jxx::NEW<Array>(2);
    (*original)[0] = ::jxx::NEW<Wrapper>(first);
    (*original)[1] = ::jxx::NEW<Wrapper>(second);
    auto restored = roundTrip(original);
    ASSERT_NE(restored, nullptr);
    ASSERT_EQ(2, restored->length);
    EXPECT_TRUE((*original)[0]->equals((*restored)[0]));
    EXPECT_TRUE((*original)[1]->equals((*restored)[1]));
}

TEST(NumericWrapperSerialization, RoundTripsAllIntegralWrappers) {
    ::jxx::initialize();
    expectIntegralRoundTrip<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(-101));
    expectIntegralRoundTrip<::jxx::lang::Short>(
        static_cast<::jxx::lang::jshort>(-30001));
    expectIntegralRoundTrip<::jxx::lang::Integer>(
        static_cast<::jxx::lang::jint>(-2000000001));
    expectIntegralRoundTrip<::jxx::lang::Long>(
        static_cast<::jxx::lang::jlong>(-9000000000000000001LL));
}

TEST(NumericWrapperSerialization, RoundTripsFloatingWrappers) {
    ::jxx::initialize();
    auto restoredFloat = roundTrip(
        ::jxx::NEW<::jxx::lang::Float>(-1234.25F));
    auto restoredDouble = roundTrip(
        ::jxx::NEW<::jxx::lang::Double>(-9876543210.125));

    ASSERT_NE(restoredFloat, nullptr);
    ASSERT_NE(restoredDouble, nullptr);
    EXPECT_FLOAT_EQ(-1234.25F, restoredFloat->floatValue());
    EXPECT_DOUBLE_EQ(-9876543210.125, restoredDouble->doubleValue());
}

TEST(NumericWrapperSerialization, PreservesFloatingSpecialValues) {
    ::jxx::initialize();
    auto nanFloat = roundTrip(::jxx::NEW<::jxx::lang::Float>(
        std::numeric_limits<::jxx::lang::jfloat>::quiet_NaN()));
    auto infinityDouble = roundTrip(::jxx::NEW<::jxx::lang::Double>(
        std::numeric_limits<::jxx::lang::jdouble>::infinity()));
    auto negativeZeroDouble = roundTrip(
        ::jxx::NEW<::jxx::lang::Double>(-0.0));

    ASSERT_NE(nanFloat, nullptr);
    ASSERT_NE(infinityDouble, nullptr);
    ASSERT_NE(negativeZeroDouble, nullptr);
    EXPECT_TRUE(std::isnan(nanFloat->floatValue()));
    EXPECT_TRUE(std::isinf(infinityDouble->doubleValue()));
    EXPECT_TRUE(std::signbit(negativeZeroDouble->doubleValue()));
}

TEST(NumericWrapperSerialization, RoundTripsRankOneArraysForAllWrappers) {
    ::jxx::initialize();
    expectRankOneArrayRoundTrip<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(1),
        static_cast<::jxx::lang::jbyte>(-2));
    expectRankOneArrayRoundTrip<::jxx::lang::Short>(
        static_cast<::jxx::lang::jshort>(3),
        static_cast<::jxx::lang::jshort>(-4));
    expectRankOneArrayRoundTrip<::jxx::lang::Integer>(5, -6);
    expectRankOneArrayRoundTrip<::jxx::lang::Long>(
        static_cast<::jxx::lang::jlong>(7),
        static_cast<::jxx::lang::jlong>(-8));
    expectRankOneArrayRoundTrip<::jxx::lang::Float>(9.5F, -10.5F);
    expectRankOneArrayRoundTrip<::jxx::lang::Double>(11.25, -12.25);
}

TEST(NumericWrapperSerialization, RoundTripsRankTwoIntegerArray) {
    using Matrix = ::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::Integer>, 2U>;
    using Row = Matrix::SubArray;
    ::jxx::initialize();

    auto original = ::jxx::NEW<Matrix>(1);
    auto row = ::jxx::NEW<Row>(2);
    (*row)[0] = ::jxx::NEW<::jxx::lang::Integer>(13);
    (*row)[1] = ::jxx::NEW<::jxx::lang::Integer>(-14);
    original->row(0) = row;

    auto restored = roundTrip(original);
    ASSERT_NE(restored, nullptr);
    const auto& restoredRow = restored->row(0);
    ASSERT_NE(restoredRow, nullptr);
    EXPECT_EQ(13, (*restoredRow)[0]->intValue());
    EXPECT_EQ(-14, (*restoredRow)[1]->intValue());
}

TEST(NumericWrapperSerialization, NoDataRestoresZeroForEveryWrapper) {
    auto byteValue = ::jxx::NEW<::jxx::lang::Byte>(
        static_cast<::jxx::lang::jbyte>(1));
    auto shortValue = ::jxx::NEW<::jxx::lang::Short>(
        static_cast<::jxx::lang::jshort>(1));
    auto intValue = ::jxx::NEW<::jxx::lang::Integer>(1);
    auto longValue = ::jxx::NEW<::jxx::lang::Long>(
        static_cast<::jxx::lang::jlong>(1));
    auto floatValue = ::jxx::NEW<::jxx::lang::Float>(1.0F);
    auto doubleValue = ::jxx::NEW<::jxx::lang::Double>(1.0);

    byteValue->readObjectNoData();
    shortValue->readObjectNoData();
    intValue->readObjectNoData();
    longValue->readObjectNoData();
    floatValue->readObjectNoData();
    doubleValue->readObjectNoData();

    EXPECT_EQ(0, byteValue->byteValue());
    EXPECT_EQ(0, shortValue->shortValue());
    EXPECT_EQ(0, intValue->intValue());
    EXPECT_EQ(0, longValue->longValue());
    EXPECT_FLOAT_EQ(0.0F, floatValue->floatValue());
    EXPECT_DOUBLE_EQ(0.0, doubleValue->doubleValue());
}

} // namespace
