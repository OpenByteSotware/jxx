#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.DateFormatSymbols.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {

::jxx::Ptr<::jxx::text::SimpleDateFormat> customized(
    const char* pattern,
    const ::jxx::Ptr<::jxx::text::DateFormatSymbols>& symbols) {
    auto result = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern), symbols);
    result->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return result;
}

TEST(SimpleDateFormatCustomSymbolParsingParity, ParsesCustomMonthAndWeekday) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto months = symbols->getMonths();
    auto weekdays = symbols->getWeekdays();
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("FirstMonth");
    (*weekdays)[5] = ::jxx::NEW<::jxx::lang::String>("FifthDay");
    symbols->setMonths(months);
    symbols->setWeekdays(weekdays);

    const auto date = customized("EEEE, MMMM d yyyy", symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("FifthDay, FirstMonth 1 1970"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}

TEST(SimpleDateFormatCustomSymbolParsingParity, ParsesCustomAmPmStrings) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto values = symbols->getAmPmStrings();
    (*values)[0] = ::jxx::NEW<::jxx::lang::String>("before-noon");
    (*values)[1] = ::jxx::NEW<::jxx::lang::String>("after-noon");
    symbols->setAmPmStrings(values);

    const auto date = customized("yyyy-MM-dd hh:mm a", symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 01:00 after-noon"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(13 * 60 * 60 * 1000LL, date->getTime());
}

TEST(SimpleDateFormatCustomSymbolParsingParity, ParsingIsCaseInsensitive) {
    auto symbols = ::jxx::NEW<::jxx::text::DateFormatSymbols>();
    auto months = symbols->getShortMonths();
    (*months)[0] = ::jxx::NEW<::jxx::lang::String>("CustomJan");
    symbols->setShortMonths(months);

    const auto date = customized("dd MMM yyyy", symbols)->parse(
        ::jxx::NEW<::jxx::lang::String>("01 customjan 1970"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(0, date->getTime());
}

} // namespace
