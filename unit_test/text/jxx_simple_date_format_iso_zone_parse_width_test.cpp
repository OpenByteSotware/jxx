#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "text/jxx.text.ParseException.h"
#include "text/jxx.text.SimpleDateFormat.h"
TEST(SimpleDateFormatIsoZoneParseParity, EnforcesXWidthGrammar) {
    auto one=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm X"));
    auto two=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm XX"));
    auto three=::jxx::NEW<::jxx::text::SimpleDateFormat>(::jxx::NEW<::jxx::lang::String>("yyyy-MM-dd HH:mm XXX"));
    EXPECT_NO_THROW(one->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +00")));
    EXPECT_NO_THROW(two->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +0000")));
    EXPECT_NO_THROW(three->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +00:00")));
    EXPECT_THROW(one->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +0000")),::jxx::text::ParseException);
    EXPECT_THROW(two->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +00:00")),::jxx::text::ParseException);
    EXPECT_THROW(three->parse(::jxx::NEW<::jxx::lang::String>("1970-01-01 00:00 +0000")),::jxx::text::ParseException);
}
