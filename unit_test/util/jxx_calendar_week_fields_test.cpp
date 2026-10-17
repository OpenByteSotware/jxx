#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

TEST(CalendarWeekParity, ConfiguresWeekDefinition)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    calendar->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    calendar->setMinimalDaysInFirstWeek(4);

    EXPECT_EQ(calendar->getFirstDayOfWeek(), ::jxx::util::Calendar::MONDAY);
    EXPECT_EQ(calendar->getMinimalDaysInFirstWeek(), 4);
}

TEST(CalendarWeekParity, RejectsInvalidWeekConfiguration)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    EXPECT_THROW(calendar->setFirstDayOfWeek(0),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(calendar->setFirstDayOfWeek(8),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(calendar->setMinimalDaysInFirstWeek(0),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(calendar->setMinimalDaysInFirstWeek(8),
                 ::jxx::lang::IllegalArgumentException);
}

TEST(CalendarWeekParity, CalculatesWeekOfYearAndMonth)
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const auto calendar = ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
    calendar->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    calendar->setMinimalDaysInFirstWeek(4);
    calendar->set(2024, ::jxx::util::Calendar::JANUARY, 8);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::WEEK_OF_YEAR), 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::WEEK_OF_MONTH), 2);
}
