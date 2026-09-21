#include <gtest/gtest.h>

#include <cmath>
#include <limits>

#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.Math.h"
#include "lang/jxx.lang.StrictMath.h"

namespace {

using ::jxx::lang::ArithmeticException;
using ::jxx::lang::Math;
using ::jxx::lang::StrictMath;

TEST(MathJava8ParityTest, FloorDivisionHandlesMinimumValueDividedByMinusOne) {
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jint>::min(),
              Math::floorDiv(std::numeric_limits<::jxx::lang::jint>::min(), -1));
    EXPECT_EQ(0, Math::floorMod(std::numeric_limits<::jxx::lang::jint>::min(), -1));
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jlong>::min(),
              Math::floorDiv(std::numeric_limits<::jxx::lang::jlong>::min(),
                             static_cast<::jxx::lang::jlong>(-1)));
    EXPECT_EQ(0LL,
              Math::floorMod(std::numeric_limits<::jxx::lang::jlong>::min(),
                             static_cast<::jxx::lang::jlong>(-1)));
}

TEST(MathJava8ParityTest, FloorDivisionAndModulusRejectZeroDivisor) {
    EXPECT_THROW(Math::floorDiv(1, 0), ArithmeticException);
    EXPECT_THROW(Math::floorMod(1, 0), ArithmeticException);
    EXPECT_THROW(Math::floorDiv(static_cast<::jxx::lang::jlong>(1),
                                static_cast<::jxx::lang::jlong>(0)),
                 ArithmeticException);
    EXPECT_THROW(Math::floorMod(static_cast<::jxx::lang::jlong>(1),
                                static_cast<::jxx::lang::jlong>(0)),
                 ArithmeticException);
    EXPECT_THROW(StrictMath::floorDiv(1, 0), ArithmeticException);
    EXPECT_THROW(StrictMath::floorMod(1, 0), ArithmeticException);
}

TEST(MathJava8ParityTest, NextDownMovesTowardNegativeInfinity) {
    EXPECT_EQ(-std::numeric_limits<::jxx::lang::jdouble>::denorm_min(),
              Math::nextDown(0.0));
    EXPECT_EQ(-std::numeric_limits<::jxx::lang::jfloat>::denorm_min(),
              Math::nextDown(0.0F));
    EXPECT_EQ(std::numeric_limits<::jxx::lang::jdouble>::max(),
              Math::nextDown(std::numeric_limits<::jxx::lang::jdouble>::infinity()));
    EXPECT_EQ(-std::numeric_limits<::jxx::lang::jdouble>::infinity(),
              Math::nextDown(-std::numeric_limits<::jxx::lang::jdouble>::infinity()));
    EXPECT_TRUE(std::isnan(Math::nextDown(
        std::numeric_limits<::jxx::lang::jdouble>::quiet_NaN())));

    EXPECT_EQ(Math::nextDown(1.0), StrictMath::nextDown(1.0));
    EXPECT_EQ(Math::nextDown(1.0F), StrictMath::nextDown(1.0F));
}

} // namespace
