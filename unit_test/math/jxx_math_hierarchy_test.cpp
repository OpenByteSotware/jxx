#include <gtest/gtest.h>
#include "math/jxx.math.BigInteger.h"
#include "math/jxx.math.BigDecimal.h"
#include "lang/jxx.lang.Comparable.h"
#include "io/jxx.io.SerializableI.h"

TEST(JxxMathHierarchyTest, BigIntegerHasExpectedHierarchyAndOrdering) {
    const auto one = ::jxx::NEW<::jxx::math::BigInteger>(1LL);
    const auto two = ::jxx::NEW<::jxx::math::BigInteger>(2LL);
    EXPECT_LT(one->compareTo(two), 0);
    EXPECT_TRUE(::jxx::math::BigInteger::Class()->isAssignableFrom(::jxx::math::BigInteger::Class()));
}

TEST(JxxMathHierarchyTest, BigDecimalCompareToIgnoresScaleDifference) {
    const auto one = ::jxx::NEW<::jxx::math::BigDecimal>(::jxx::NEW<::jxx::lang::String>("1.0"));
    const auto equal = ::jxx::NEW<::jxx::math::BigDecimal>(::jxx::NEW<::jxx::lang::String>("1.00"));
    EXPECT_EQ(one->compareTo(equal), 0);
}
