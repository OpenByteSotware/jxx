#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigDecimal.h"
#include "math/jxx.math.BigInteger.h"

namespace {

template <typename T>
::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& value) {
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
        output->writeObject(::jxx::CAST<::jxx::lang::Object>(value));
        output->flush();
    }

    auto inputBytes =
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(inputBytes);
    return ::jxx::CAST<T>(input->readObject());
}

TEST(JxxMathSerialization, RoundTripsBigIntegerExactly) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<::jxx::math::BigInteger>(
        ::jxx::NEW<::jxx::lang::String>("-123456789012345678901234567890"));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(original->equals(restored));
    EXPECT_EQ(original->toString()->utf8(), restored->toString()->utf8());
}

TEST(JxxMathSerialization, RoundTripsBigDecimalValueAndScale) {
    ::jxx::initialize();
    auto original = ::jxx::NEW<::jxx::math::BigDecimal>(
        ::jxx::NEW<::jxx::lang::String>("-1234567890.0012300"));

    auto restored = roundTrip(original);

    ASSERT_NE(restored, nullptr);
    EXPECT_TRUE(original->equals(restored));
    EXPECT_EQ(original->scale(), restored->scale());
    EXPECT_EQ(original->toString()->utf8(), restored->toString()->utf8());
}

TEST(JxxMathSerialization, ResetsNoDataStateToZero) {
    auto integer = ::jxx::NEW<::jxx::math::BigInteger>(
        ::jxx::NEW<::jxx::lang::String>("42"));
    integer->readObjectNoData();
    EXPECT_EQ("0", integer->toString()->utf8());

    auto decimal = ::jxx::NEW<::jxx::math::BigDecimal>(
        ::jxx::NEW<::jxx::lang::String>("42.50"));
    decimal->readObjectNoData();
    EXPECT_EQ("0", decimal->toString()->utf8());
    EXPECT_EQ(0, decimal->scale());
}

} // namespace
