#include <gtest/gtest.h>

#include <limits>

#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.Math.h"

TEST(JxxLangMathStage, ProvidesConstantsAndBasicFunctions) {
    EXPECT_DOUBLE_EQ(::jxx::lang::Math::PI, 3.14159265358979323846);
    EXPECT_DOUBLE_EQ(::jxx::lang::Math::E, 2.7182818284590452354);
    EXPECT_DOUBLE_EQ(::jxx::lang::Math::sqrt(81.0), 9.0);
    EXPECT_EQ(::jxx::lang::Math::floorDiv(-7, 3), -3);
    EXPECT_EQ(::jxx::lang::Math::floorMod(-7, 3), 2);
}

TEST(JxxLangMathStage, DetectsExactArithmeticOverflow) {
    EXPECT_THROW(
        ::jxx::lang::Math::addExact(
            std::numeric_limits<::jxx::lang::jint>::max(), 1),
        ::jxx::lang::ArithmeticException);
    EXPECT_THROW(
        ::jxx::lang::Math::multiplyExact(
            std::numeric_limits<::jxx::lang::jlong>::max(),
            static_cast<::jxx::lang::jlong>(2)),
        ::jxx::lang::ArithmeticException);
}

TEST(JxxLangMathStage, RandomUsesRequiredRange) {
    for (int index = 0; index < 100; ++index) {
        const auto value = ::jxx::lang::Math::random();
        EXPECT_GE(value, 0.0);
        EXPECT_LT(value, 1.0);
    }
}
