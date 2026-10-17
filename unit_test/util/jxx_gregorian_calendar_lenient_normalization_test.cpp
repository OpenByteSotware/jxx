#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

namespace {

::jxx::Ptr<::jxx::util::GregorianCalendar> utcCalendar()
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    return ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
}

} // namespace

TEST(GregorianCalendarLeniencyParity, NormalizesOverflowingMonthAndDay)
{
    const auto calendar = utcCalendar();
    calendar->setLenient(true);
    calendar->set(2024, ::jxx::util::Calendar::JANUARY + 12, 1);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2025);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::JANUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 1);

    calendar->set(2024, ::jxx::util::Calendar::FEBRUARY, 30);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::MARCH);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 1);
}

TEST(GregorianCalendarLeniencyParity, NormalizesTimeOverflow)
{
    const auto calendar = utcCalendar();
    calendar->setLenient(true);
    calendar->set(2024, ::jxx::util::Calendar::JANUARY, 1, 25, 61, 61);

    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::HOUR_OF_DAY), 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MINUTE), 2);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::SECOND), 1);
}

TEST(GregorianCalendarLeniencyParity, StrictModeRejectsInvalidFields)
{
    const auto calendar = utcCalendar();
    calendar->setLenient(false);

    EXPECT_THROW(
        calendar->set(2024, ::jxx::util::Calendar::FEBRUARY, 30),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(
        calendar->set(2024, ::jxx::util::Calendar::JANUARY, 1, 24, 0, 0),
        ::jxx::lang::IllegalArgumentException);
}
