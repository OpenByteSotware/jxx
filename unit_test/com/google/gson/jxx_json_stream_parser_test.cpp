#include <gtest/gtest.h>

#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
#include "com/google/gson/jxx.com.google.gson.JsonStreamParser.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"

TEST(JsonStreamParserParity, ParsesConsecutiveValues)
{
    const auto source = ::jxx::NEW<::jxx::lang::String>("1 true \\\"value\\\"");
    const auto parser = ::jxx::NEW<::com::google::gson::JsonStreamParser>(source);

    EXPECT_TRUE(parser->hasNext());
    ASSERT_NE(parser->next(), nullptr);
    EXPECT_TRUE(parser->hasNext());
    ASSERT_NE(parser->next(), nullptr);
    EXPECT_TRUE(parser->hasNext());
    ASSERT_NE(parser->next(), nullptr);
    EXPECT_FALSE(parser->hasNext());
}

TEST(JsonStreamParserParity, NextAfterEndThrows)
{
    const auto source = ::jxx::NEW<::jxx::lang::String>("null");
    const auto parser = ::jxx::NEW<::com::google::gson::JsonStreamParser>(source);

    ASSERT_NE(parser->next(), nullptr);
    EXPECT_THROW(parser->next(), ::jxx::util::NoSuchElementException);
}
