#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.SimpleDateFormat.h"
#include "util/jxx.util.Date.h"
#include "util/jxx.util.TimeZone.h"

namespace {
::jxx::Ptr<::jxx::text::SimpleDateFormat> utcFormat(const char* pattern) {
    auto format = ::jxx::NEW<::jxx::text::SimpleDateFormat>(
        ::jxx::NEW<::jxx::lang::String>(pattern));
    format->setTimeZone(::jxx::util::TimeZone::getTimeZone(
        ::jxx::NEW<::jxx::lang::String>("GMT")));
    return format;
}
}

TEST(SimpleDateFormatMillisecondParsingParity, ParsesTerminalMilliseconds) {
    const auto date = utcFormat("yyyy-MM-dd HH:mm:ss.SSS")->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00:00.978"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(978, date->getTime());
}

TEST(SimpleDateFormatMillisecondParsingParity, ParsesMillisecondsBeforeZone) {
    const auto date = utcFormat("yyyy-MM-dd'T'HH:mm:ss.SSSXXX")->parse(
        ::jxx::NEW<::jxx::lang::String>("1970-01-01T01:00:00.125+01:00"));
    ASSERT_NE(nullptr, date);
    EXPECT_EQ(125, date->getTime());
}

TEST(SimpleDateFormatMillisecondParsingParity,
     RejectsOutOfRangeMilliseconds)
{
    auto format = utcFormat("yyyy-MM-dd HH:mm:ss.SSS");
    format->setLenient(false);

    EXPECT_THROW(
        format->parse(
            ::jxx::NEW<::jxx::lang::String>(
                "1970-01-01 00:00:00.1000")),
        ::jxx::text::ParseException);
}