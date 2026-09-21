#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.TimeZone.h"
namespace {
TEST(GregorianCalendarParityTest, UsesTimeZoneAbstractionForLocalFields){auto zone=::jxx::util::TimeZone::getTimeZone(::jxx::NEW<::jxx::lang::String>("GMT-08:00"));auto calendar=::jxx::NEW<::jxx::util::GregorianCalendar>(zone);calendar->set(2021,0,1,0,0,0);EXPECT_EQ(2021,calendar->get(::jxx::util::Calendar::YEAR));EXPECT_EQ(0,calendar->get(::jxx::util::Calendar::MONTH));EXPECT_EQ(1,calendar->get(::jxx::util::Calendar::DAY_OF_MONTH));}
TEST(GregorianCalendarParityTest, LeapYearsAndMonthMaximumsAreGregorian){auto calendar=::jxx::NEW<::jxx::util::GregorianCalendar>(2024,1,1);EXPECT_TRUE(calendar->isLeapYear(2024));EXPECT_FALSE(calendar->isLeapYear(1900));EXPECT_TRUE(calendar->isLeapYear(2000));EXPECT_EQ(29,calendar->getActualMaximum(::jxx::util::Calendar::DAY_OF_MONTH));}
TEST(GregorianCalendarParityTest, AddMonthClampsDayAndRollDoesNotCarry){auto calendar=::jxx::NEW<::jxx::util::GregorianCalendar>(2024,0,31);calendar->add(::jxx::util::Calendar::MONTH,1);EXPECT_EQ(1,calendar->get(::jxx::util::Calendar::MONTH));EXPECT_EQ(29,calendar->get(::jxx::util::Calendar::DAY_OF_MONTH));calendar->roll(::jxx::util::Calendar::MONTH,11);EXPECT_EQ(0,calendar->get(::jxx::util::Calendar::MONTH));EXPECT_EQ(2024,calendar->get(::jxx::util::Calendar::YEAR));}
TEST(GregorianCalendarParityTest, GregorianChangeIsDefensivelyCopied){auto calendar=::jxx::NEW<::jxx::util::GregorianCalendar>();auto change=::jxx::NEW<::jxx::util::Date>(0);calendar->setGregorianChange(change);change->setTime(1);EXPECT_EQ(0,calendar->getGregorianChange()->getTime());EXPECT_TRUE(calendar->getCalendarType()->utf8()=="gregory");}
}
