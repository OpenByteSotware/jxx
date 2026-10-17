#include <gtest/gtest.h>

#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.TimeZone.h"

TEST(CalendarFieldParity, ExposesStandardFieldAndValueConstants)
{
    EXPECT_EQ(::jxx::util::Calendar::ERA, 0);
    EXPECT_EQ(::jxx::util::Calendar::DAY_OF_YEAR, 6);
    EXPECT_EQ(::jxx::util::Calendar::AM_PM, 9);
    EXPECT_EQ(::jxx::util::Calendar::ZONE_OFFSET, 15);
    EXPECT_EQ(::jxx::util::Calendar::FIELD_COUNT, 17);
    EXPECT_EQ(::jxx::util::Calendar::DATE,
              ::jxx::util::Calendar::DAY_OF_MONTH);
    EXPECT_EQ(::jxx::util::Calendar::JANUARY, 0);
    EXPECT_EQ(::jxx::util::Calendar::DECEMBER, 11);
    EXPECT_EQ(::jxx::util::Calendar::SUNDAY, 1);
    EXPECT_EQ(::jxx::util::Calendar::SATURDAY, 7);
}

TEST(CalendarFieldParity, ExtractsUtcEpochFields)
{
    const auto calendar = ::jxx::util::Calendar::getInstance(
        ::jxx::util::TimeZone::getTimeZone(
            ::jxx::NEW<::jxx::lang::String>("UTC")));
    calendar->setTimeInMillis(0);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::ERA), 1);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 1970);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::JANUARY), 0);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_YEAR), 1);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::AM_PM),
              ::jxx::util::Calendar::AM);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::HOUR), 0);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::ZONE_OFFSET), 0);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DST_OFFSET), 0);
}
