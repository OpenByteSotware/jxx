#include <gtest/gtest.h>
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(DateTimeZoneBackendParity, TimezoneOffsetUsesJxxDefaultZone) {
    const auto date = ::jxx::NEW<::jxx::util::Date>(0);
    const auto zone = ::jxx::util::TimeZone::getDefault();
    ASSERT_NE(nullptr, zone);
    EXPECT_EQ(-zone->getOffset(0) / 60000, date->getTimezoneOffset());
}
