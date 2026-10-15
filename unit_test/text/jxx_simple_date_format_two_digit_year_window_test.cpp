#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> yearFormat(const char* pattern) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatTwoDigitYearParity, HonorsConfiguredCenturyWindow) {
    auto format = yearFormat("MM/dd/yy");
    format->set2DigitYearStart(
        ::jxx::NEW<::jxx::util::Date>(50, 0, 1));
    EXPECT_EQ(1950, format->parse(::jxx::NEW<::jxx::lang::String>(
        "01/01/50"))->getYear() + 1900);
    EXPECT_EQ(2049, format->parse(::jxx::NEW<::jxx::lang::String>(
        "01/01/49"))->getYear() + 1900);
}

TEST(SimpleDateFormatTwoDigitYearParity, UsesExactStartInstantBoundary) {
    auto format = yearFormat("MM/dd/yy HH:mm:ss");
    format->set2DigitYearStart(
        ::jxx::NEW<::jxx::util::Date>(50, 5, 15, 12, 0, 0));
    EXPECT_EQ(2050, format->parse(::jxx::NEW<::jxx::lang::String>(
        "06/15/50 11:59:59"))->getYear() + 1900);
    EXPECT_EQ(1950, format->parse(::jxx::NEW<::jxx::lang::String>(
        "06/15/50 12:00:00"))->getYear() + 1900);
}

TEST(SimpleDateFormatTwoDigitYearParity, FourDigitYearIsNotWindowed) {
    auto format = yearFormat("MM/dd/yyyy");
    format->set2DigitYearStart(
        ::jxx::NEW<::jxx::util::Date>(50, 0, 1));
    EXPECT_EQ(49, format->parse(::jxx::NEW<::jxx::lang::String>(
        "01/01/0049"))->getYear() + 1900);
}
