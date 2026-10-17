#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

namespace {

::jxx::Ptr<::jxx::util::GregorianCalendar> calendarAt(
    ::jxx::lang::jint year,
    ::jxx::lang::jint month,
    ::jxx::lang::jint day)
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const auto calendar =
        ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
    calendar->set(year, month, day, 10, 20, 30);
    return calendar;
}

} // namespace

TEST(GregorianCalendarAddParity, AddsMonthAndClampsDay)
{
    const auto calendar = calendarAt(
        2024, ::jxx::util::Calendar::JANUARY, 31);
    calendar->add(::jxx::util::Calendar::MONTH, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::FEBRUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 29);
}

TEST(GregorianCalendarAddParity, AddsYearAndClampsLeapDay)
{
    const auto calendar = calendarAt(
        2024, ::jxx::util::Calendar::FEBRUARY, 29);
    calendar->add(::jxx::util::Calendar::YEAR, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2025);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::FEBRUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 28);
}

TEST(GregorianCalendarAddParity, AddsWeekAndDayFields)
{
    const auto calendar = calendarAt(
        2024, ::jxx::util::Calendar::JANUARY, 1);
    calendar->add(::jxx::util::Calendar::WEEK_OF_YEAR, 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 15);

    calendar->add(::jxx::util::Calendar::DAY_OF_YEAR, 1);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 16);
}

TEST(GregorianCalendarAddParity, AddsAmPmAndHour)
{
    const auto calendar = calendarAt(
        2024, ::jxx::util::Calendar::JANUARY, 1);
    calendar->add(::jxx::util::Calendar::AM_PM, 1);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::HOUR_OF_DAY), 22);

    calendar->add(::jxx::util::Calendar::HOUR, 3);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::HOUR_OF_DAY), 1);
}

TEST(GregorianCalendarAddParity, RejectsUnsupportedField)
{
    const auto calendar = calendarAt(
        2024, ::jxx::util::Calendar::JANUARY, 1);
    EXPECT_THROW(calendar->add(::jxx::util::Calendar::ZONE_OFFSET, 1),
                 ::jxx::lang::IllegalArgumentException);
}
