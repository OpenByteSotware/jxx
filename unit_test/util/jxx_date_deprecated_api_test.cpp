#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Date.h"

TEST(DateDeprecatedApiParity, UtcFactoryUsesYearOffsetAndZeroBasedMonth) {
    const auto millis = ::jxx::util::Date::UTC(70, 0, 1, 0, 0, 0);
    EXPECT_EQ(0, millis);
}

TEST(DateDeprecatedApiParity, DeprecatedFieldsNormalizeThroughLocalCalendar) {
    auto date = ::jxx::NEW<::jxx::util::Date>(126, 0, 15, 10, 20, 30);
    EXPECT_EQ(126, date->getYear());
    EXPECT_EQ(0, date->getMonth());
    EXPECT_EQ(15, date->getDate());
    EXPECT_EQ(10, date->getHours());
    EXPECT_EQ(20, date->getMinutes());
    EXPECT_EQ(30, date->getSeconds());

    date->setMonth(12);
    EXPECT_EQ(127, date->getYear());
    EXPECT_EQ(0, date->getMonth());
}

TEST(DateDeprecatedApiParity, ParsesAndFormatsGmtValue) {
    const auto value = ::jxx::NEW<::jxx::lang::String>(
        "01 Jan 1970 00:00:00 GMT");
    const auto date = ::jxx::NEW<::jxx::util::Date>(value);
    EXPECT_EQ(0, date->getTime());
    EXPECT_TRUE(date->toGMTString()->equals(
        ::jxx::NEW<::jxx::lang::String>("01 Jan 1970 00:00:00 GMT")));
}
