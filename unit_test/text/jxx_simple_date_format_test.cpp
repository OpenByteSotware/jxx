#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(SimpleDateFormatParity, FormatsCommonPatternsAndQuotes) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd 'at' HH:mm:ss.SSS Z"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    EXPECT_TRUE(format->format(::jxx::NEW<::jxx::util::Date>(0))->equals(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 at 00:00:00.000 +0000")));
}

TEST(SimpleDateFormatParity, ApplyPatternAndDefensiveYearStartCopy) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy"));
    format->applyPattern(::jxx::NEW<::jxx::lang::String>("yy"));
    EXPECT_TRUE(format->toPattern()->equals(
        ::jxx::NEW<::jxx::lang::String>("yy")));
    auto start = ::jxx::NEW<::jxx::util::Date>(1234);
    format->set2DigitYearStart(start);
    start->setTime(9999);
    EXPECT_EQ(1234, format->get2DigitYearStart()->getTime());
}

TEST(SimpleDateFormatParity, RejectsUnknownPatternLetters) {
    EXPECT_THROW(
        ::jxx::NEW<::jxx::text::SimpleDateFormat>(
            ::jxx::NEW<::jxx::lang::String>("yyyy-q")),
        ::jxx::lang::IllegalArgumentException);
}
