#include <gtest/gtest.h>

#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.Calendar.h"
#include "util/jxx.util.GregorianCalendar.h"
#include "util/jxx.util.Locale.h"
#include "util/jxx.util.TimeZone.h"

TEST(CalendarFactoryParity, DefaultFactoryReturnsGregorianCalendar)
{
    const auto calendar = ::jxx::util::Calendar::getInstance();
    ASSERT_NE(calendar, nullptr);
    EXPECT_NE(::jxx::CAST<::jxx::util::GregorianCalendar>(calendar), nullptr);
}

TEST(CalendarFactoryParity, TimeZoneOverloadPreservesRequestedZone)
{
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT+05:30"));
    const auto calendar = ::jxx::util::Calendar::getInstance(zone);

    ASSERT_NE(calendar, nullptr);
    ASSERT_NE(calendar->getTimeZone(), nullptr);
    EXPECT_TRUE(calendar->getTimeZone()->equals(
        ::jxx::CAST<::jxx::lang::Object>(zone)));
}

TEST(CalendarFactoryParity, LocaleAndCombinedOverloadsReturnGregorianCalendar)
{
    const auto locale = ::jxx::util::Locale::US;
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("UTC"));

    EXPECT_NE(
        ::jxx::CAST<::jxx::util::GregorianCalendar>(
            ::jxx::util::Calendar::getInstance(locale)),
        nullptr);
    EXPECT_NE(
        ::jxx::CAST<::jxx::util::GregorianCalendar>(
            ::jxx::util::Calendar::getInstance(zone, locale)),
        nullptr);
}

TEST(CalendarFactoryParity, NullFactoryArgumentsThrow)
{
    const ::jxx::Ptr<::jxx::util::TimeZone> zone;
    const ::jxx::Ptr<::jxx::util::Locale> locale;

    EXPECT_THROW(
        ::jxx::util::Calendar::getInstance(zone),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::util::Calendar::getInstance(locale),
        ::jxx::lang::NullPointerException);
    EXPECT_THROW(
        ::jxx::util::Calendar::getInstance(zone, locale),
        ::jxx::lang::NullPointerException);
}
