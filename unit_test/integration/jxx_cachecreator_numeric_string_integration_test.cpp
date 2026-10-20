#include <gtest/gtest.h>

#include <limits>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Color.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Math.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(CacheCreatorSdkIntegration, ResolutionStringUsesIntegerToStringAndConcatenation)
{
    ::jxx::initialize();
    auto resolution = ::jxx::lang::Integer::toString(1920)
        ->concat(::jxx::NEW<::jxx::lang::String>("x"))
        ->concat(::jxx::lang::Integer::toString(1080));
    ASSERT_NE(resolution, nullptr);
    EXPECT_EQ(resolution->utf8(), "1920x1080");
}

TEST(CacheCreatorSdkIntegration, ProportionalSizingRoundBehaviorMatchesSourceCases)
{
    ::jxx::initialize();
    EXPECT_EQ(::jxx::lang::Math::round(39.49), 39);
    EXPECT_EQ(::jxx::lang::Math::round(39.50), 40);
    EXPECT_EQ(::jxx::lang::Math::round(-1.50), -1);
    EXPECT_EQ(::jxx::lang::Math::round(-1.51), -2);
    EXPECT_EQ(::jxx::lang::Math::round(
        std::numeric_limits<::jxx::lang::jdouble>::quiet_NaN()), 0);
    EXPECT_EQ(::jxx::lang::Math::round(
        std::numeric_limits<::jxx::lang::jdouble>::infinity()),
        std::numeric_limits<::jxx::lang::jlong>::max());
    EXPECT_EQ(::jxx::lang::Math::round(
        -std::numeric_limits<::jxx::lang::jdouble>::infinity()),
        std::numeric_limits<::jxx::lang::jlong>::min());
}

TEST(CacheCreatorSdkIntegration, RgbaConstructorValidatesAllChannels)
{
    ::jxx::initialize();
    EXPECT_NO_THROW(::jxx::NEW<::jxx::awt::Color>(0, 0, 0, 0));
    EXPECT_NO_THROW(::jxx::NEW<::jxx::awt::Color>(255, 255, 255, 255));
    EXPECT_THROW(::jxx::NEW<::jxx::awt::Color>(-1, 0, 0, 0),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(::jxx::NEW<::jxx::awt::Color>(0, 256, 0, 0),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(::jxx::NEW<::jxx::awt::Color>(0, 0, -1, 0),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(::jxx::NEW<::jxx::awt::Color>(0, 0, 0, 256),
        ::jxx::lang::IllegalArgumentException);
}

} // namespace
