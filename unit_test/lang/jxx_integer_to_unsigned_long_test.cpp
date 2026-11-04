#include <gtest/gtest.h>

#include <cstdint>
#include <limits>

#include "lang/jxx.lang.Integer.h"

TEST(IntegerToUnsignedLongTest, PreservesNonnegativeValues)
{
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(0),
              ::jxx::lang::Integer::toUnsignedLong(0));
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(1),
              ::jxx::lang::Integer::toUnsignedLong(1));
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(2147483647LL),
              ::jxx::lang::Integer::toUnsignedLong(
                  ::jxx::lang::Integer::MAX_VALUE));
}

TEST(IntegerToUnsignedLongTest, ZeroExtendsNegativeValues)
{
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(4294967295LL),
              ::jxx::lang::Integer::toUnsignedLong(-1));
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(4294967291LL),
              ::jxx::lang::Integer::toUnsignedLong(-5));
    EXPECT_EQ(static_cast<::jxx::lang::jlong>(2147483648LL),
              ::jxx::lang::Integer::toUnsignedLong(
                  ::jxx::lang::Integer::MIN_VALUE));
}

TEST(IntegerToUnsignedLongTest, MatchesUnsignedThirtyTwoBitRepresentation)
{
    const ::jxx::lang::jint values[] = {
        static_cast<::jxx::lang::jint>(0),
        static_cast<::jxx::lang::jint>(42),
        static_cast<::jxx::lang::jint>(-42),
        ::jxx::lang::Integer::MIN_VALUE,
        ::jxx::lang::Integer::MAX_VALUE,
    };

    for (const auto value : values) {
        const auto expected = static_cast<::jxx::lang::jlong>(
            static_cast<std::uint64_t>(
                static_cast<std::uint32_t>(value)));
        EXPECT_EQ(expected,
                  ::jxx::lang::Integer::toUnsignedLong(value));
    }
}
