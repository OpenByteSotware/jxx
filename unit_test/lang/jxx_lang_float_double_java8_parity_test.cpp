#include <gtest/gtest.h>
#include <cmath>
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Float.h"

namespace {
using ::jxx::lang::Double;
using ::jxx::lang::Float;

TEST(FloatJava8ParityTest, DoubleConstructorNarrowsToFloat) {
    const auto value = ::jxx::NEW<Float>(1.25);
    EXPECT_EQ(1.25F, value->floatValue());
}

TEST(FloatJava8ParityTest, FiniteAndFunctionalMethodsPreserveIeeeEdges) {
    EXPECT_TRUE(Float::isFinite(1.0F));
    EXPECT_FALSE(Float::isFinite(Float::POSITIVE_INFINITY));
    EXPECT_FALSE(Float::isFinite(Float::NaN));
    EXPECT_EQ(3.0F, Float::sum(1.0F, 2.0F));
    EXPECT_TRUE(std::signbit(Float::min(0.0F, -0.0F)));
    EXPECT_FALSE(std::signbit(Float::max(0.0F, -0.0F)));
    EXPECT_TRUE(std::isnan(Float::min(Float::NaN, 1.0F)));
    EXPECT_TRUE(std::isnan(Float::max(1.0F, Float::NaN)));
}

TEST(DoubleJava8ParityTest, FiniteAndFunctionalMethodsPreserveIeeeEdges) {
    EXPECT_TRUE(Double::isFinite(1.0));
    EXPECT_FALSE(Double::isFinite(Double::NEGATIVE_INFINITY));
    EXPECT_FALSE(Double::isFinite(Double::NaN));
    EXPECT_EQ(3.0, Double::sum(1.0, 2.0));
    EXPECT_TRUE(std::signbit(Double::min(0.0, -0.0)));
    EXPECT_FALSE(std::signbit(Double::max(0.0, -0.0)));
    EXPECT_TRUE(std::isnan(Double::min(Double::NaN, 1.0)));
    EXPECT_TRUE(std::isnan(Double::max(1.0, Double::NaN)));
}

TEST(FloatDoubleJava8ParityTest, CompareDistinguishesSignedZeroAndOrdersNanLast) {
    EXPECT_LT(Float::compare(-0.0F, 0.0F), 0);
    EXPECT_GT(Float::compare(Float::NaN, Float::POSITIVE_INFINITY), 0);
    EXPECT_LT(Double::compare(-0.0, 0.0), 0);
    EXPECT_GT(Double::compare(Double::NaN, Double::POSITIVE_INFINITY), 0);
}
} // namespace
