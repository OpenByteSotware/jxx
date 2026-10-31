#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.TimeZone.h"
#include "jxx.initialize.h"

TEST(TimeZoneJava8Parity, ReconstructedFixedOffsetZonePreservesValueSemantics)
{
    jxx::initialize();
    const auto first = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const auto second = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));

    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first.get(), second.get());
    EXPECT_TRUE(first->hasSameRules(second));
    EXPECT_TRUE(first->equals(::jxx::CAST<::jxx::lang::Object>(second)));
    EXPECT_EQ(first->hashCode(), second->hashCode());
}

TEST(TimeZoneJava8Parity, SameRulesDoesNotImplyEquality)
{
    jxx::initialize();
    const auto utc = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const auto gmt = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT"));

    ASSERT_NE(utc, nullptr);
    ASSERT_NE(gmt, nullptr);
    EXPECT_TRUE(utc->hasSameRules(gmt));
    EXPECT_FALSE(utc->equals(::jxx::CAST<::jxx::lang::Object>(gmt)));
}
