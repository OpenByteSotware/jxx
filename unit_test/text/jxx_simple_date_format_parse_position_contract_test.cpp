#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParsePosition.h"
#include "text/jxx.text.SimpleDateFormat.h"

TEST(SimpleDateFormatParsePositionParity, ConsumesLongestValidPrefix) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto position = ::jxx::NEW<::jxx::text::ParsePosition>(0);
    const auto result = format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 trailing"), position);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(10, position->getIndex());
    EXPECT_EQ(-1, position->getErrorIndex());
}

TEST(SimpleDateFormatParsePositionParity, PreservesStartIndexOnFailure) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto position = ::jxx::NEW<::jxx::text::ParsePosition>(3);
    const auto result = format->parse(
        ::jxx::NEW<::jxx::lang::String>("xxxnot-a-date"), position);
    EXPECT_EQ(nullptr, result);
    EXPECT_EQ(3, position->getIndex());
    EXPECT_GE(position->getErrorIndex(), 3);
}

TEST(SimpleDateFormatParsePositionParity, ClearsStaleErrorAfterSuccess) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto position = ::jxx::NEW<::jxx::text::ParsePosition>(0);
    position->setErrorIndex(7);
    const auto result = format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01"), position);
    ASSERT_NE(nullptr, result);
    EXPECT_EQ(10, position->getIndex());
    EXPECT_EQ(-1, position->getErrorIndex());
}
