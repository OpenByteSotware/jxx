#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> newFormat() {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd"));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatEqualityParity, EqualStateHasEqualHashCode) {
    const auto left = newFormat();
    const auto right = newFormat();
    EXPECT_TRUE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
    EXPECT_EQ(left->hashCode(), right->hashCode());
}

TEST(SimpleDateFormatEqualityParity, SymbolsParticipateInEquality) {
    const auto left = newFormat();
    const auto right = newFormat();
    auto symbols = right->getDateFormatSymbols();
    auto months = symbols->getMonths();
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("MonthOne");
    symbols->setMonths(months);
    right->setDateFormatSymbols(symbols);
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}

TEST(SimpleDateFormatEqualityParity, TwoDigitYearStartParticipatesInEquality) {
    const auto left = newFormat();
    const auto right = newFormat();
    right->set2DigitYearStart(::jxx::NEW<::jxx::util::Date>(0));
    EXPECT_FALSE(left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}

TEST(SimpleDateFormatEqualityParity, CloneRetainsEqualityAndHashCode) {
    const auto original = newFormat();
    const auto clone = ::jxx::CAST<::jxx::text::SimpleDateFormat>(original->clone());
    ASSERT_NE(nullptr, clone);
    EXPECT_TRUE(original->equals(::jxx::CAST<::jxx::lang::Object>(clone)));
    EXPECT_EQ(original->hashCode(), clone->hashCode());
}
