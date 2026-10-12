#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "math/jxx.math.BigDecimal.h"
#include "math/jxx.math.BigInteger.h"
#include "math/jxx.math.ClassInitialization.h"

TEST(JxxMathClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    const auto bigDecimalClass = ::jxx::math::BigDecimal::Class();
    const auto bigIntegerClass = ::jxx::math::BigInteger::Class();

    ASSERT_NE(bigDecimalClass, nullptr);
    ASSERT_NE(bigIntegerClass, nullptr);
    EXPECT_NE(bigDecimalClass, bigIntegerClass);
}

TEST(JxxMathClassInitialization, RepeatedInitializationIsIdempotent)
{
    const auto bigDecimalBefore = ::jxx::math::BigDecimal::Class();
    const auto bigIntegerBefore = ::jxx::math::BigInteger::Class();

    EXPECT_NO_THROW(::jxx::math::initializeClasses());
    EXPECT_NO_THROW(::jxx::math::initializeClasses());

    EXPECT_EQ(bigDecimalBefore, ::jxx::math::BigDecimal::Class());
    EXPECT_EQ(bigIntegerBefore, ::jxx::math::BigInteger::Class());
}
