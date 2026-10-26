#include <gtest/gtest.h>
#include "math/jxx.math.BigInteger.h"
#include "lang/jxx.lang.String.h"
TEST(BigIntegerParityTest, ConstantsAndCoreArithmetic)
{
	EXPECT_EQ(0, ::jxx::math::BigInteger::ZERO->intValue());
	EXPECT_EQ(1, ::jxx::math::BigInteger::ONE->intValue());
	EXPECT_EQ(10, ::jxx::math::BigInteger::TEN->intValue());
	auto a = ::jxx::NEW<::jxx::math::BigInteger>(::jxx::NEW<::jxx::lang::String>("100"));
	auto b = ::jxx::NEW<::jxx::math::BigInteger>(::jxx::NEW<::jxx::lang::String>("7"));
	EXPECT_EQ(107, a->add(b)->intValue());
	EXPECT_EQ(93, a->subtract(b)->intValue());
	EXPECT_EQ(700, a->multiply(b)->intValue());
	EXPECT_EQ(14, a->divide(b)->intValue());
	EXPECT_EQ(2, a->remainder(b)->intValue());
}
TEST(BigIntegerParityTest, RadixPowAndGcd)
{
	auto ff = ::jxx::NEW<::jxx::math::BigInteger>(::jxx::NEW<::jxx::lang::String>("ff"), 16);
	EXPECT_EQ(255, ff->intValue()); 
	EXPECT_EQ(1000, ::jxx::math::BigInteger::TEN->pow(3)->intValue());
	EXPECT_EQ(5, ff->gcd(::jxx::math::BigInteger::TEN)->intValue());
}
