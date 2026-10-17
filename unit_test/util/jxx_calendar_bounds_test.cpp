#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"

TEST(CalendarBoundsParity, ReportsStandardFieldBounds)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();

    EXPECT_EQ(calendar->getMinimum(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::JANUARY);
    EXPECT_EQ(calendar->getMaximum(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::DECEMBER);
    EXPECT_EQ(calendar->getMinimum(::jxx::util::Calendar::DAY_OF_MONTH), 1);
    EXPECT_EQ(calendar->getMaximum(::jxx::util::Calendar::DAY_OF_MONTH), 31);
    EXPECT_EQ(calendar->getLeastMaximum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 28);
    EXPECT_EQ(calendar->getMaximum(::jxx::util::Calendar::DAY_OF_YEAR), 366);
    EXPECT_EQ(calendar->getLeastMaximum(
                  ::jxx::util::Calendar::DAY_OF_YEAR), 365);
    EXPECT_EQ(calendar->getMaximum(::jxx::util::Calendar::HOUR_OF_DAY), 23);
    EXPECT_EQ(calendar->getMaximum(::jxx::util::Calendar::MILLISECOND), 999);
}

TEST(CalendarBoundsParity, GreatestMinimumUsesFieldMinimum)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    EXPECT_EQ(calendar->getGreatestMinimum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 1);
    EXPECT_EQ(calendar->getGreatestMinimum(
                  ::jxx::util::Calendar::MONTH), 0);
}

TEST(CalendarBoundsParity, InvalidFieldThrows)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    EXPECT_THROW(calendar->getMinimum(-1),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(calendar->getMaximum(::jxx::util::Calendar::FIELD_COUNT),
                 ::jxx::lang::IllegalArgumentException);
}
