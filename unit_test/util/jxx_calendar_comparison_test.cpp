#include <gtest/gtest.h>

#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Calendar.h"

TEST(CalendarParity, BeforeAndAfterUseTimelineOrder)
{
    const auto earlier = ::jxx::NEW<::jxx::util::Calendar>();
    const auto later = ::jxx::NEW<::jxx::util::Calendar>();
    earlier->setTimeInMillis(1000);
    later->setTimeInMillis(2000);

    EXPECT_TRUE(earlier->before(::jxx::CAST<::jxx::lang::Object>(later)));
    EXPECT_FALSE(earlier->after(::jxx::CAST<::jxx::lang::Object>(later)));
    EXPECT_TRUE(later->after(::jxx::CAST<::jxx::lang::Object>(earlier)));
}

TEST(CalendarParity, BeforeAndAfterRejectNonCalendarWithoutThrowing)
{
    const auto calendar = ::jxx::NEW<::jxx::util::Calendar>();
    const auto object = ::jxx::NEW<::jxx::lang::Object>();

    EXPECT_FALSE(calendar->before(object));
    EXPECT_FALSE(calendar->after(object));
    EXPECT_FALSE(calendar->before(nullptr));
    EXPECT_FALSE(calendar->after(nullptr));
}

TEST(CalendarParity, EqualsAndHashCodeTrackCalendarState)
{
    const auto left = ::jxx::NEW<::jxx::util::Calendar>();
    const auto right = ::jxx::NEW<::jxx::util::Calendar>();
    left->setTimeInMillis(123456789);
    right->setTimeInMillis(123456789);
    right->setTimeZone(left->getTimeZone());

    EXPECT_TRUE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
    EXPECT_EQ(left->hashCode(), right->hashCode());

    right->setTimeInMillis(123456790);
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}
