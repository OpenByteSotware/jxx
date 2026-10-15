#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(SimpleDateFormatPatternTokenizerParity, PreservesQuotedLettersAndApostrophes) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy 'year' MM ''dd"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    EXPECT_EQ("1970 year 01 '01", format->format(
        ::jxx::NEW<::jxx::util::Date>(0))->utf8());
}

TEST(SimpleDateFormatPatternTokenizerParity, RejectsUnterminatedQuote) {
    EXPECT_THROW(
        ::jxx::NEW<::jxx::text::SimpleDateFormat>(
            ::jxx::NEW<::jxx::lang::String>("yyyy 'unterminated")),
        ::jxx::lang::IllegalArgumentException);
}

TEST(SimpleDateFormatPatternTokenizerParity, ValidatesIsoWidthOutsideQuotesOnly) {
    EXPECT_THROW(
        ::jxx::NEW<::jxx::text::SimpleDateFormat>(
            ::jxx::NEW<::jxx::lang::String>("XXXX")),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_NO_THROW(
        ::jxx::NEW<::jxx::text::SimpleDateFormat>(
            ::jxx::NEW<::jxx::lang::String>("'XXXX'")));
}

TEST(SimpleDateFormatPatternTokenizerParity, QuotedYearDoesNotActivateCenturyWindow) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("'yy' yyyy"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    EXPECT_EQ("yy 1970", format->format(
        ::jxx::NEW<::jxx::util::Date>(0))->utf8());
}
