#include <gtest/gtest.h>

#include <vector>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"

TEST(SimpleDateFormatZoneNameParity, ParsesCustomizedTerminalName) {
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
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm:ss z"), symbols);
    const auto date = format->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00 CGT"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}
