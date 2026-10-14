#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> eraFormat(
    const ::jxx::Ptr<::jxx::text::DateFormatSymbols>& symbols) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("G yyyy-MM-dd HH:mm:ss"), symbols);
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatEraParsingParity, ParsesCustomizedAdEra) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto eras = symbols->getEras();
    (*eras)[1] = ::jxx::NEW<::jxx::lang::String>("CommonEra");
    symbols->setEras(eras);
    const auto date = eraFormat(symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("CommonEra 1970-01-01 00:00:00"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}

TEST(SimpleDateFormatEraParsingParity, ParsesCustomizedBcEra) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto eras = symbols->getEras();
    (*eras)[0] = ::jxx::NEW<::jxx::lang::String>("BeforeCommonEra");
    symbols->setEras(eras);
    const auto date = eraFormat(symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("BeforeCommonEra 0001-01-01 00:00:00"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(-62167219200000LL, date->getTime());
}

TEST(SimpleDateFormatEraParsingParity, PreservesMillisecondsWithEra) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("G yyyy-MM-dd HH:mm:ss.SSS"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    const auto date = format->parse(
        ::jxx::NEW<::jxx::lang::String>("AD 1970-01-01 00:00:00.125"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(125, date->getTime());
}
