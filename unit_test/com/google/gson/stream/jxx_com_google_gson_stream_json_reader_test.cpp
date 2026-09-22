#include <gtest/gtest.h>

#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonToken.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.String.h"

namespace {

::jxx::Ptr<::com::google::gson::stream::JsonReader> reader(const char* json) {
    return ::jxx::NEW<::com::google::gson::stream::JsonReader>(
        ::jxx::NEW<::jxx::io::StringReader>(
            ::jxx::NEW<::jxx::lang::String>(json)));
}

TEST(GsonJsonReaderTest, ReadsNestedObjectAndArray) {
    const auto json = reader(R"({"name":"device","values":[1,2,3],"enabled":true})");
    json->beginObject();
    EXPECT_EQ("name", json->nextName()->utf8());
    EXPECT_EQ("device", json->nextString()->utf8());
    EXPECT_EQ("values", json->nextName()->utf8());
    json->beginArray();
    EXPECT_EQ(1, json->nextInt());
    EXPECT_EQ(2, json->nextInt());
    EXPECT_EQ(3, json->nextInt());
    EXPECT_FALSE(json->hasNext());
    json->endArray();
    EXPECT_EQ("enabled", json->nextName()->utf8());
    EXPECT_TRUE(json->nextBoolean());
    json->endObject();
    EXPECT_EQ(::com::google::gson::stream::JsonToken::END_DOCUMENT, json->peek());
}

TEST(GsonJsonReaderTest, DecodesUnicodeEscapesAndSurrogatePairs) {
    const auto json = reader(R"(["A\u03A9","\uD83D\uDE00"])");
    json->beginArray();
    EXPECT_EQ(std::string("A\xCE\xA9"), json->nextString()->utf8());
    EXPECT_EQ(std::string("\xF0\x9F\x98\x80"), json->nextString()->utf8());
    json->endArray();
}

TEST(GsonJsonReaderTest, SkipsUnknownNestedValueAndTracksPath) {
    const auto json = reader(R"({"keep":1,"skip":{"nested":[2,3]},"tail":4})");
    json->beginObject();
    EXPECT_EQ("keep", json->nextName()->utf8());
    EXPECT_EQ(1, json->nextInt());
    EXPECT_EQ("skip", json->nextName()->utf8());
    EXPECT_EQ("$.skip", json->getPath()->utf8());
    json->skipValue();
    EXPECT_EQ("tail", json->nextName()->utf8());
    EXPECT_EQ(4, json->nextInt());
    json->endObject();
}

TEST(GsonJsonReaderTest, RejectsMalformedUnicodeEscape) {
    const auto json = reader(R"(["\uZZZZ"])");
    json->beginArray();
    EXPECT_THROW(json->nextString(), ::com::google::gson::stream::MalformedJsonException);
}

} // namespace
