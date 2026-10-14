#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
TEST(SimpleDateFormatZoneNameParity, ParsesCustomizedTerminalName) {
 using S=::jxx::text::DateFormatSymbols; using R=S::ZoneStringArray::SubArray;
 std::vector<::jxx::Ptr<R>> rows; rows.push_back(::jxx::NEW<R>(std::initializer_list<::jxx::Ptr<::jxx::lang::String>>{
 ::jxx::NEW<::jxx::lang::String>("GMT"),::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),::jxx::NEW<::jxx::lang::String>("CGT"),::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),::jxx::NEW<::jxx::lang::String>("CGT")}));
 auto symbols=::jxx::NEW<S>(); symbols->setZoneStrings(::jxx::NEW<S::ZoneStringArray>(std::move(rows)));
 auto f=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm:ss z"),symbols);
 EXPECT_EQ(0,f->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00 CGT"))->getTime());
}
