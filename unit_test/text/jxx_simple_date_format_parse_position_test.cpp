#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParsePosition.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(SimpleDateFormatParsePositionParity, ParsesPrefixAndAdvancesIndex) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    auto position = ::jxx::NEW<::jxx::text::ParsePosition>(2);

    const auto date = format->parse(
        ::jxx::NEW<::jxx::lang::String>("##1970-01-01 trailing"), position);

    ASSERT_NE(nullptr, date);
    EXPECT_EQ(12, position->getIndex());
    EXPECT_EQ(-1, position->getErrorIndex());
    EXPECT_EQ(0, date->getTime());
}

TEST(SimpleDateFormatParsePositionParity, FailurePreservesIndexAndSetsErrorIndex) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto position = ::jxx::NEW<::jxx::text::ParsePosition>(1);

    const auto date = format->parse(
        ::jxx::NEW<::jxx::lang::String>("#not-a-date"), position);

    EXPECT_EQ(nullptr, date);
    EXPECT_EQ(1, position->getIndex());
    EXPECT_EQ(1, position->getErrorIndex());
}
