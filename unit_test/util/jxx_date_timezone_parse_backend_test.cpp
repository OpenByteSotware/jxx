#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"
TEST(DateTimezoneParseBackendParity, LocalLegacyParseUsesJxxDefaultZone) {
    const auto parsed = ::jxx::util::Date::parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00"));
    const auto expected = -static_cast<::jxx::lang::jlong>(::jxx::util::TimeZone::getDefault()->getOffset(0));
    EXPECT_EQ(expected, parsed);
}
TEST(DateTimezoneParseBackendParity, GmtParseRemainsUtc) {
    EXPECT_EQ(0, ::jxx::util::Date::parse(::jxx::NEW<::jxx::lang::String>("01 Jan 1970 00:00:00 GMT")));
}
