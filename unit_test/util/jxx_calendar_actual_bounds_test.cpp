#include <gtest/gtest.h>

#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

TEST(CalendarActualBoundsParity, DispatchesThroughCalendarReference)
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const ::jxx::Ptr<::jxx::util::Calendar> calendar =
        ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
    const auto gregorian =
        ::jxx::CAST<::jxx::util::GregorianCalendar>(calendar);
    gregorian->set(2024, ::jxx::util::Calendar::FEBRUARY, 10);

    EXPECT_EQ(calendar->getActualMinimum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 1);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 29);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_YEAR), 366);
}

TEST(CalendarActualBoundsParity, HandlesCommonAndLeapYears)
{
    const auto calendar = ::jxx::NEW<::jxx::util::GregorianCalendar>(
        2023, ::jxx::util::Calendar::FEBRUARY, 1);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 28);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_YEAR), 365);

    calendar->set(2024, ::jxx::util::Calendar::FEBRUARY, 1);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_MONTH), 29);
    EXPECT_EQ(calendar->getActualMaximum(
                  ::jxx::util::Calendar::DAY_OF_YEAR), 366);
}
