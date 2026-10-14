#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(SimpleDateFormatZoneNameParity, UsesCustomizedShortAndLongNames) {
    using Symbols = ::jxx::text::DateFormatSymbols;
    using Row = Symbols::ZoneStringArray::SubArray;
    std::vector<::jxx::Ptr<Row>> rows;
    rows.push_back(::jxx::NEW<Row>(
        std::initializer_list<::jxx::Ptr<::jxx::lang::String>>{
            ::jxx::NEW<::jxx::lang::String>("GMT"),
            ::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),
            ::jxx::NEW<::jxx::lang::String>("CGT"),
            ::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),
            ::jxx::NEW<::jxx::lang::String>("CGT")}));
    auto symbols = ::jxx::NEW<Symbols>();
    symbols->setZoneStrings(
        ::jxx::NEW<Symbols::ZoneStringArray>(std::move(rows)));
    auto shortFormat = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("z"), symbols);
    auto longFormat = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("zzzz"), symbols);
    const auto zone = ::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT"));
    shortFormat->setTimeZone(zone);
    longFormat->setTimeZone(zone);
    EXPECT_EQ("CGT", shortFormat->format(
        ::jxx::NEW<::jxx::util::Date>(0))->utf8());
    EXPECT_EQ("Custom Greenwich", longFormat->format(
        ::jxx::NEW<::jxx::util::Date>(0))->utf8());
}
