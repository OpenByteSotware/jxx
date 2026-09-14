#include <gtest/gtest.h>

#include <limits>
#include <type_traits>

#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.StrictMath.h"

TEST(JxxLangStrictMathStage, IsFinalAndNotConstructible) {
    EXPECT_TRUE((std::is_final_v<::jxx::lang::StrictMath>));
    EXPECT_FALSE((std::is_default_constructible_v<::jxx::lang::StrictMath>));
}

TEST(JxxLangStrictMathStage, ProvidesConstantsAndCoreOperations) {
    EXPECT_DOUBLE_EQ(::jxx::lang::StrictMath::PI, 3.14159265358979323846);
    EXPECT_DOUBLE_EQ(::jxx::lang::StrictMath::E, 2.7182818284590452354);
    EXPECT_DOUBLE_EQ(::jxx::lang::StrictMath::sqrt(81.0), 9.0);
    EXPECT_EQ(::jxx::lang::StrictMath::floorDiv(-7, 3), -3);
    EXPECT_EQ(::jxx::lang::StrictMath::floorMod(-7, 3), 2);
}

TEST(JxxLangStrictMathStage, PreservesSignedZero) {
    const auto result = ::jxx::lang::StrictMath::signum(-0.0);
    EXPECT_TRUE(std::signbit(result));
}

TEST(JxxLangStrictMathStage, DetectsExactArithmeticOverflow) {
    EXPECT_THROW(
        ::jxx::lang::StrictMath::addExact(
            std::numeric_limits<::jxx::lang::jint>::max(), 1),
        ::jxx::lang::ArithmeticException);
}
