#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

TEST(CumulativeFourImportParity, DateFormatSymbolsPublishesSupportedLocales) {
    const auto locales = ::jxx::text::DateFormatSymbols::getAvailableLocales();
    ASSERT_NE(nullptr, locales);
    EXPECT_EQ(8, locales->length);
}
TEST(CumulativeFourImportParity, SimpleDateFormatSymbolsAffectEquality) {
    auto left = ::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto right = ::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    auto symbols = right->getDateFormatSymbols();
    auto months = symbols->getMonths();
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("MonthOne");
    symbols->setMonths(months);
    right->setDateFormatSymbols(symbols);
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}
TEST(CumulativeFourImportParity, SimpleDateFormatEqualStateHasEqualHash) {
    auto left = ::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    const auto right = ::jxx::CAST<::jxx::text::SimpleDateFormat>(left->clone());
    ASSERT_NE(nullptr, right);
    EXPECT_TRUE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
    EXPECT_EQ(left->hashCode(), right->hashCode());
}
