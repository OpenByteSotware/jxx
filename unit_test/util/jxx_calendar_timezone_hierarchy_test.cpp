#include <gtest/gtest.h>

#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"
#include "util/jxx.util.SimpleTimeZone.h"

TEST(JxxCalendarTimeZoneHierarchyTest, CalendarDeclaresJava8Interfaces) {
    EXPECT_TRUE(::jxx::lang::Cloneable::Class()->isAssignableFrom(
        ::jxx::util::Calendar::Class()));
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::Calendar::Class()));
    EXPECT_TRUE(::jxx::util::Calendar::Class()->isAssignableFrom(
        ::jxx::util::GregorianCalendar::Class()));
}

TEST(JxxCalendarTimeZoneHierarchyTest, TimeZoneDeclaresJava8Interfaces) {
    EXPECT_TRUE(::jxx::lang::Cloneable::Class()->isAssignableFrom(
        ::jxx::util::TimeZone::Class()));
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::TimeZone::Class()));
    EXPECT_TRUE(::jxx::util::TimeZone::Class()->isAssignableFrom(
        ::jxx::util::SimpleTimeZone::Class()));
}

TEST(JxxCalendarTimeZoneHierarchyTest, CalendarComparisonUsesEpochMillis) {
    const auto first = ::jxx::NEW<::jxx::util::GregorianCalendar>();
    const auto second = ::jxx::NEW<::jxx::util::GregorianCalendar>();
    first->setTimeInMillis(10);
    second->setTimeInMillis(20);
    EXPECT_LT(first->compareTo(second), 0);
    EXPECT_GT(second->compareTo(first), 0);
    EXPECT_EQ(first->compareTo(first), 0);
    EXPECT_THROW(first->compareTo(nullptr), ::jxx::lang::NullPointerException);
}
