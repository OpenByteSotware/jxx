#include <gtest/gtest.h>

#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "lang/jxx.lang.String.h"
#include "com/google/gson/jxx.com.google.gson.JsonElement.h"
TEST(JsonParserUnicodeEscape, DecodesBmpEscape) {
    auto parsed = ::com::google::gson::JsonParser::parseString(
        ::jxx::NEW<::jxx::lang::String>("\"A\\u00E9Z\""));
    EXPECT_EQ("A\xC3\xA9Z", parsed->getAsString()->utf8());
}

TEST(JsonParserUnicodeEscape, DecodesSupplementarySurrogatePair) {
    auto parsed = ::com::google::gson::JsonParser::parseString(
        ::jxx::NEW<::jxx::lang::String>("\"\\uD83D\\uDE00\""));
    EXPECT_EQ("\xF0\x9F\x98\x80", parsed->getAsString()->utf8());
}

TEST(JsonParserUnicodeEscape, RejectsMalformedAndUnpairedEscapes) {
    using Parser = ::com::google::gson::JsonParser;
    using String = ::jxx::lang::String;
    using Error = ::com::google::gson::JsonSyntaxException;

    EXPECT_THROW(Parser::parseString(::jxx::NEW<String>("\"\\u12G4\"")), Error);
    EXPECT_THROW(Parser::parseString(::jxx::NEW<String>("\"\\uD83D\"")), Error);
    EXPECT_THROW(Parser::parseString(::jxx::NEW<String>("\"\\uDE00\"")), Error);
    EXPECT_THROW(Parser::parseString(
        ::jxx::NEW<String>("\"\\uD83D\\u0041\"")), Error);
}
