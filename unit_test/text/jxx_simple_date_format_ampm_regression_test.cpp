#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> customFormat(
    const ::jxx::Ptr<::jxx::text::DateFormatSymbols>& symbols) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd hh:mm a"), symbols);
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatCustomSymbolParsingParity, ParsesCustomAmPmStringsPortable) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto values = symbols->getAmPmStrings();
    (*values)[0] = ::jxx::NEW<::jxx::lang::String>("before-noon");
    (*values)[1] = ::jxx::NEW<::jxx::lang::String>("after-noon");
    symbols->setAmPmStrings(values);

    const auto date = customFormat(symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 01:00 after-noon"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(13 * 60 * 60 * 1000LL, date->getTime());
}

TEST(SimpleDateFormatCustomSymbolParsingParity, HandlesPortableMidnightAndNoon) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto values = symbols->getAmPmStrings();
    (*values)[0] = ::jxx::NEW<::jxx::lang::String>("ante");
    (*values)[1] = ::jxx::NEW<::jxx::lang::String>("post");
    symbols->setAmPmStrings(values);

    const auto midnight = customFormat(symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 12:00 ante"));
    const auto noon = customFormat(symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 12:00 post"));
    ASSERT_NE(nullptr, midnight);
    ASSERT_NE(nullptr, noon);
    EXPECT_EQ(0, midnight->getTime());
    EXPECT_EQ(12 * 60 * 60 * 1000LL, noon->getTime());
}
