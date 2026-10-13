#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.TimeZone.h"

TEST(SimpleDateFormatParseParity, ParsesNumericDateTimeInConfiguredZone) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm:ss"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));

    const auto date = format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}

TEST(SimpleDateFormatParseParity, RejectsTrailingInput) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    EXPECT_THROW(
        format->parse(::jxx::NEW<::jxx::lang::String>("2026-10-13 extra")),
        ::jxx::text::ParseException);
}

TEST(SimpleDateFormatParseParity, ParseExceptionExposesErrorOffset) {
    const auto message = ::jxx::NEW<::jxx::lang::String>("bad date");
    const ::jxx::text::ParseException exception(message, 4);
    EXPECT_EQ(4, exception.getErrorOffset());
}
