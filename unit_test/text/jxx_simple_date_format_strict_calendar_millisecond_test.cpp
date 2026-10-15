#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.Date.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> utcFormat(const char* pattern) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatStrictCalendarParity, RejectsDayBeyondMonthLength) {
    auto format = utcFormat("yyyy-MM-dd");
    format->setLenient(false);
    EXPECT_THROW(format->parse(::jxx::NEW<::jxx::lang::String>(
        "2023-04-31")), ::jxx::text::ParseException);
}

TEST(SimpleDateFormatStrictCalendarParity, AppliesGregorianLeapYearRules) {
    auto format = utcFormat("yyyy-MM-dd");
    format->setLenient(false);
    EXPECT_NO_THROW(format->parse(::jxx::NEW<::jxx::lang::String>(
        "2024-02-29")));
    EXPECT_THROW(format->parse(::jxx::NEW<::jxx::lang::String>(
        "2023-02-29")), ::jxx::text::ParseException);
    EXPECT_THROW(format->parse(::jxx::NEW<::jxx::lang::String>(
        "1900-02-29")), ::jxx::text::ParseException);
    EXPECT_NO_THROW(format->parse(::jxx::NEW<::jxx::lang::String>(
        "2000-02-29")));
}

TEST(SimpleDateFormatMillisecondParity, LenientParsingNormalizesOverflow) {
    auto format = utcFormat("yyyy-MM-dd HH:mm:ss S");
    format->setLenient(true);
    const auto normalized = format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00 1000"));
    ASSERT_NE(nullptr, normalized);
    EXPECT_EQ((static_cast<::jxx::lang::jlong>(1000)), (normalized->getTime()));
    
    format->setLenient(false);
    EXPECT_THROW(format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00 1000")),
        ::jxx::text::ParseException);
}
