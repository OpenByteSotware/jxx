#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> gmtFormat(const char* pattern) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatTokenFormattingParity, FormatsQuotedAndEscapedLiterals) {
    auto format = gmtFormat("yyyy 'year' MM ''dd 'at' HH:mm:ss.SSS");
    EXPECT_EQ("1970 year 01 '01 at 00:00:00.000",
        format->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
}

TEST(SimpleDateFormatTokenFormattingParity, FormatsAdjacentFieldRuns) {
    auto format = gmtFormat("yyyyMMddHHmmss");
    EXPECT_EQ("19700101000000",
        format->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
}

TEST(SimpleDateFormatTokenFormattingParity, PreservesLiteralPunctuation) {
    auto format = gmtFormat("'['yyyy-MM-dd'T'HH:mm:ss']'");
    EXPECT_EQ("[1970-01-01T00:00:00]",
        format->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
}

TEST(SimpleDateFormatTokenFormattingParity, FormatsAllHourVariants) {
    auto format = gmtFormat("H HH k kk K KK h hh a");
    EXPECT_EQ("0 00 24 24 0 00 12 12 AM",
        format->format(::jxx::NEW<::jxx::util::Date>(0))->utf8());
}
