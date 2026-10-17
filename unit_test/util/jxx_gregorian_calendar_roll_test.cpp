#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

namespace {

::jxx::Ptr<::jxx::util::GregorianCalendar> utcCalendar(
    ::jxx::lang::jint year,
    ::jxx::lang::jint month,
    ::jxx::lang::jint day,
    ::jxx::lang::jint hour = 0)
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    const auto calendar =
        ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
    calendar->set(year, month, day, hour, 30, 15);
    return calendar;
}

} // namespace

TEST(GregorianCalendarRollParity, MonthRollDoesNotChangeYear)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::DECEMBER, 31);
    calendar->roll(::jxx::util::Calendar::MONTH, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::JANUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 31);
}

TEST(GregorianCalendarRollParity, MonthRollClampsDayWithinYear)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::JANUARY, 31);
    calendar->roll(::jxx::util::Calendar::MONTH, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::FEBRUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 29);
}

TEST(GregorianCalendarRollParity, DayRollStaysWithinMonth)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::JANUARY, 31);
    calendar->roll(::jxx::util::Calendar::DAY_OF_MONTH, true);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::JANUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 1);
}

TEST(GregorianCalendarRollParity, HourRollStaysWithinDay)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::JANUARY, 10, 23);
    calendar->roll(::jxx::util::Calendar::HOUR_OF_DAY, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 10);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::HOUR_OF_DAY), 0);
}

TEST(GregorianCalendarRollParity, BooleanOverloadRollsDown)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::JANUARY, 1);
    calendar->roll(::jxx::util::Calendar::DAY_OF_MONTH, false);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 31);
}

TEST(GregorianCalendarRollParity, RejectsUnsupportedField)
{
    const auto calendar = utcCalendar(
        2024, ::jxx::util::Calendar::JANUARY, 1);
    EXPECT_THROW(calendar->roll(::jxx::util::Calendar::ZONE_OFFSET, 1),
                 ::jxx::lang::IllegalArgumentException);
}
