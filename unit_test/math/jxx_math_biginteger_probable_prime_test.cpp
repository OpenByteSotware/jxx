#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigInteger.h"

namespace
{
    ::jxx::Ptr<::jxx::math::BigInteger> bigInteger(const char* value)
    {
        return ::jxx::NEW<::jxx::math::BigInteger>(
            ::jxx::NEW<::jxx::lang::String>(value));
    }
}

TEST(BigIntegerProbablePrimeTest, RecognizesSmallPrimesAndComposites)
{
    EXPECT_TRUE(bigInteger("2")->isProbablePrime(1));
    EXPECT_TRUE(bigInteger("3")->isProbablePrime(1));
    EXPECT_TRUE(bigInteger("5")->isProbablePrime(10));
    EXPECT_TRUE(bigInteger("97")->isProbablePrime(20));

    EXPECT_FALSE(bigInteger("0")->isProbablePrime(1));
    EXPECT_FALSE(bigInteger("1")->isProbablePrime(1));
    EXPECT_FALSE(bigInteger("-7")->isProbablePrime(1));
    EXPECT_FALSE(bigInteger("4")->isProbablePrime(1));
    EXPECT_FALSE(bigInteger("9")->isProbablePrime(10));
    EXPECT_FALSE(bigInteger("25")->isProbablePrime(20));
}

TEST(BigIntegerProbablePrimeTest, RecognizesLargerValues)
{
    EXPECT_TRUE(bigInteger("104729")->isProbablePrime(20));
    EXPECT_TRUE(bigInteger("32416190071")->isProbablePrime(40));

    EXPECT_FALSE(bigInteger("104728")->isProbablePrime(20));
    EXPECT_FALSE(bigInteger("3215031751")->isProbablePrime(40));
}

TEST(BigIntegerProbablePrimeTest, NonPositiveCertaintyReturnsTrue)
{
    EXPECT_TRUE(bigInteger("4")->isProbablePrime(0));
    EXPECT_TRUE(bigInteger("4")->isProbablePrime(-1));
    EXPECT_TRUE(bigInteger("-7")->isProbablePrime(0));
}
