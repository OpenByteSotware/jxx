#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"
#include "jxx.initialize.h"

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
    jxx::initialize();
    auto format = yearFormat("MM/dd/yy");
    const auto start = ::jxx::util::Date::UTC(50, 0, 1, 0, 0, 0);
    format->set2DigitYearStart(::jxx::NEW<::jxx::util::Date>(start));

    const auto atStart = format->parse(
        ::jxx::NEW<::jxx::lang::String>("01/01/50"));
    const auto priorSuffix = format->parse(
        ::jxx::NEW<::jxx::lang::String>("01/01/49"));

    EXPECT_EQ(start, atStart->getTime());
    EXPECT_EQ("01/01/50", format->format(atStart)->utf8());
    EXPECT_EQ("01/01/49", format->format(priorSuffix)->utf8());
    EXPECT_GT(priorSuffix->getTime(), atStart->getTime());
}

TEST(SimpleDateFormatTwoDigitYearParity, UsesExactStartInstantBoundary) {
    jxx::initialize();
    auto format = yearFormat("MM/dd/yy HH:mm:ss");
    const auto start = ::jxx::util::Date::UTC(50, 5, 15, 12, 0, 0);
    format->set2DigitYearStart(::jxx::NEW<::jxx::util::Date>(start));

    const auto before = format->parse(
        ::jxx::NEW<::jxx::lang::String>("06/15/50 11:59:59"));
    const auto boundary = format->parse(
        ::jxx::NEW<::jxx::lang::String>("06/15/50 12:00:00"));

    EXPECT_EQ(start, boundary->getTime());
    EXPECT_GT(before->getTime(), boundary->getTime());
    EXPECT_EQ("06/15/50 11:59:59", format->format(before)->utf8());
    EXPECT_EQ("06/15/50 12:00:00", format->format(boundary)->utf8());
}

TEST(SimpleDateFormatYearParity, ParsesAndFormatsProlepticYearWithoutWindowing) {
    jxx::initialize();
    auto format = yearFormat("MM/dd/yyyy");
    const auto parsed = format->parse(
        ::jxx::NEW<::jxx::lang::String>("01/01/0049"));
    ASSERT_NE(nullptr, parsed);
    EXPECT_EQ("01/01/0049", format->format(parsed)->utf8());
}
