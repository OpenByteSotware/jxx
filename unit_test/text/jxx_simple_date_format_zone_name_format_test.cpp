#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"
TEST(SimpleDateFormatZoneNameParity, UsesCustomizedShortAndLongNames) {
 using S=::jxx::text::DateFormatSymbols; using R=S::ZoneStringArray::SubArray;
 std::vector<::jxx::Ptr<R>> rows;
 rows.push_back(::jxx::NEW<R>(std::initializer_list<::jxx::Ptr<::jxx::lang::String>>{
  ::jxx::NEW<::jxx::lang::String>("GMT"),::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),
  ::jxx::NEW<::jxx::lang::String>("CGT"),::jxx::NEW<::jxx::lang::String>("Custom Greenwich"),
  ::jxx::NEW<::jxx::lang::String>("CGT")}));
 auto symbols=::jxx::NEW<S>(); symbols->setZoneStrings(::jxx::NEW<S::ZoneStringArray>(std::move(rows)));
 auto shortF=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("z"),symbols);
 auto longF=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("zzzz"),symbols);
 auto zone=::jxx::util::TimeZone::getTimeZone(::jxx::NEW<::jxx::lang::String>("GMT"));
 shortF->setTimeZone(zone); longF->setTimeZone(zone);
 EXPECT_EQ("CGT",shortF->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
 EXPECT_EQ("Custom Greenwich",longF->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
}
