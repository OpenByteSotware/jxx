#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Calendar.h"

TEST(CalendarLeniencyParity, DefaultsToLenient)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    EXPECT_TRUE(calendar->isLenient());
}

TEST(CalendarLeniencyParity, SetLenientUpdatesState)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    calendar->setLenient(false);
    EXPECT_FALSE(calendar->isLenient());
    calendar->setLenient(true);
    EXPECT_TRUE(calendar->isLenient());
}

TEST(CalendarLeniencyParity, EqualityIncludesCalendarConfiguration)
{
    const auto left = ::jxx::util::Calendar::getInstance();
    const auto right = ::jxx::util::Calendar::getInstance();
    left->setTimeInMillis(1234);
    right->setTimeInMillis(1234);
    right->setTimeZone(left->getTimeZone());

    EXPECT_TRUE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
    EXPECT_EQ(left->hashCode(), right->hashCode());

    right->setLenient(false);
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));

    right->setLenient(true);
    right->setFirstDayOfWeek(::jxx::util::Calendar::MONDAY);
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}
