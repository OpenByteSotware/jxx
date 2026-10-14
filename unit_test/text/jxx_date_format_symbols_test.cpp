#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(DateFormatSymbolsParity, SimpleDateFormatUsesDefensiveCustomSymbols) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto months = symbols->getMonths();
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("MonthOne");
    symbols->setMonths(months);
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("MMMM"), symbols);
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("ChangedAgain");
    EXPECT_TRUE(format->format(::jxx::NEW<::jxx::util::Date>(0))->equals(
        ::jxx::NEW<::jxx::lang::String>("MonthOne")));
}
