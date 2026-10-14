#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {

::jxx::Ptr<::jxx::text::SimpleDateFormat> formatter(const char* pattern) {
    auto value = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern));
    value->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return value;
}

TEST(SimpleDateFormatWeekParity, FormatsIsoWeekYearAtCalendarBoundary) {
    const auto date = ::jxx::NEW<::jxx::util::Date>(
        ::jxx::util::Date::UTC(115, 11, 31, 0, 0, 0));
    EXPECT_TRUE(formatter("YYYY-'W'ww-u")->format(date)->equals(
        ::jxx::NEW<::jxx::lang::String>("2015-W53-4")));
}

TEST(SimpleDateFormatWeekParity, FormatsWeekInMonthAndDayOfWeekInMonth) {
    const auto date = ::jxx::NEW<::jxx::util::Date>(
        ::jxx::util::Date::UTC(126, 9, 14, 0, 0, 0));
    EXPECT_TRUE(formatter("W F")->format(date)->equals(
        ::jxx::NEW<::jxx::lang::String>("2 2")));
}

TEST(SimpleDateFormatTimeZoneParity, ParsesRfc822Offset) {
    const auto date = formatter("yyyy-MM-dd HH:mm Z")->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 01:00 +0100"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}

TEST(SimpleDateFormatTimeZoneParity, ParsesIso8601OffsetAndZulu) {
    const auto offsetDate = formatter("yyyy-MM-dd'T'HH:mmXXX")->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01T01:30+01:30"));
    const auto zuluDate = formatter("yyyy-MM-dd'T'HH:mmX")->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01T00:00Z"));
    ASSERT_NE(nullptr, offsetDate);
    ASSERT_NE(nullptr, zuluDate);
    EXPECT_EQ(0, offsetDate->getTime());
    EXPECT_EQ(0, zuluDate->getTime());
}

} // namespace
