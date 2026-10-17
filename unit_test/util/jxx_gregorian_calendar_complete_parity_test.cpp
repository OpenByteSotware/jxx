#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::util::GregorianCalendar> utcCalendar() {
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));
    return ::jxx::NEW<::jxx::util::GregorianCalendar>(zone);
}
} // namespace

TEST(GregorianCalendarCompleteParity, SupportsWeekDates) {
    const auto calendar = utcCalendar();
    calendar->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    calendar->setMinimalDaysInFirstWeek(4);
    calendar->setWeekDate(2024, 1, ::jxx::util::Calendar::MONDAY);
    EXPECT_TRUE(calendar->isWeekDateSupported());
    EXPECT_EQ(calendar->getWeekYear(), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::YEAR), 2024);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::MONTH),
              ::jxx::util::Calendar::JANUARY);
    EXPECT_EQ(calendar->get(::jxx::util::Calendar::DAY_OF_MONTH), 1);
    EXPECT_EQ(calendar->getWeeksInWeekYear(), 52);
}

TEST(GregorianCalendarCompleteParity, WeekDateValidatesArguments) {
    const auto calendar = utcCalendar();
    EXPECT_THROW(calendar->setWeekDate(
                     2024, 0, ::jxx::util::Calendar::MONDAY),
                 ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(calendar->setWeekDate(2024, 1, 0),
                 ::jxx::lang::IllegalArgumentException);
}

TEST(GregorianCalendarCompleteParity, GregorianChangeUsesDefensiveCopies) {
    const auto calendar = utcCalendar();
    const auto supplied = ::jxx::NEW<::jxx::util::Date>(1234);
    calendar->setGregorianChange(supplied);
    supplied->setTime(5678);
    const auto first = calendar->getGregorianChange();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first->getTime(), 1234);
    first->setTime(9999);
    EXPECT_EQ(calendar->getGregorianChange()->getTime(), 1234);
}

TEST(GregorianCalendarCompleteParity, CloneHasIndependentState) {
    const auto source = utcCalendar();
    source->set(2024, ::jxx::util::Calendar::MARCH, 10);
    source->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    source->setMinimalDaysInFirstWeek(4);
    source->setLenient(false);
    const auto cloned = ::jxx::CAST<::jxx::util::GregorianCalendar>(
        source->clone());
    ASSERT_NE(cloned, nullptr);
    EXPECT_TRUE(source->equals(
        ::jxx::CAST<::jxx::lang::Object>(cloned)));
    cloned->add(::jxx::util::Calendar::DAY_OF_MONTH, 1);
    EXPECT_NE(source->getTimeInMillis(), cloned->getTimeInMillis());
}
