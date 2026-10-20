#include <gtest/gtest.h>

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "io/jxx.io.StringReader.h"
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.ArrayList.h"

namespace {
using ErasedMap = ::com::google::gson::internal::LinkedTreeMap<
    ::jxx::lang::Object, ::jxx::lang::Object>;
using ErasedList = ::jxx::util::ArrayList<::jxx::lang::Object>;

::jxx::Ptr<::jxx::lang::String> text(const char* value)
{
    return ::jxx::NEW<::jxx::lang::String>(value);
}

::jxx::Ptr<::com::google::gson::stream::JsonReader> reader(const char* value)
{
    return ::jxx::NEW<::com::google::gson::stream::JsonReader>(
        ::jxx::NEW<::jxx::io::StringReader>(text(value)));
}

::jxx::Ptr<::jxx::lang::Object> get(
    const ::jxx::Ptr<ErasedMap>& map, const char* key)
{
    return map->get(::jxx::CAST<::jxx::lang::Object>(text(key)));
}
}

TEST(GsonFromJsonClassTest, MaterializesRawLinkedTreeMapRecursively)
{
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    const auto value = gson->fromJson(
        reader("{\"name\":\"car\",\"enabled\":true,\"count\":7,"
               "\"ratio\":2.5,\"nested\":{\"ip\":\"10.0.0.1 /24\"},"
               "\"items\":[\"a\",null] }"),
        ErasedMap::Class());

    const auto map = ::jxx::CAST<ErasedMap>(value);
    ASSERT_NE(map, nullptr);
    EXPECT_EQ(::jxx::CAST<::jxx::lang::String>(get(map, "name"))->utf8(), "car");
    EXPECT_TRUE(::jxx::CAST<::jxx::lang::Boolean>(get(map, "enabled"))->booleanValue());
    EXPECT_EQ(::jxx::CAST<::jxx::lang::Long>(get(map, "count"))->longValue(), 7);
    EXPECT_DOUBLE_EQ(::jxx::CAST<::jxx::lang::Double>(get(map, "ratio"))->doubleValue(), 2.5);

    const auto nested = ::jxx::CAST<ErasedMap>(get(map, "nested"));
    ASSERT_NE(nested, nullptr);
    EXPECT_EQ(::jxx::CAST<::jxx::lang::String>(get(nested, "ip"))->utf8(),
              "10.0.0.1 /24");

    const auto items = ::jxx::CAST<ErasedList>(get(map, "items"));
    ASSERT_NE(items, nullptr);
    ASSERT_EQ(items->size(), 2);
    EXPECT_EQ(::jxx::CAST<::jxx::lang::String>(items->get(0))->utf8(), "a");
    EXPECT_EQ(items->get(1), nullptr);
}

TEST(GsonFromJsonClassTest, PreservesObjectInsertionOrder)
{
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    const auto map = ::jxx::CAST<ErasedMap>(
        gson->fromJson(reader("{\"first\":1,\"second\":2}"), ErasedMap::Class()));
    ASSERT_NE(map, nullptr);
    const auto iterator = map->entrySet()->iterator();
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(::jxx::CAST<::jxx::lang::String>(iterator->next()->getKey())->utf8(),
              "first");
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(::jxx::CAST<::jxx::lang::String>(iterator->next()->getKey())->utf8(),
              "second");
}

TEST(GsonFromJsonClassTest, ReturnsNullForJsonNull)
{
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    EXPECT_EQ(gson->fromJson(reader("null"), ErasedMap::Class()), nullptr);
}

TEST(GsonFromJsonClassTest, RejectsNonObjectForLinkedTreeMap)
{
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    EXPECT_THROW(gson->fromJson(reader("[]"), ErasedMap::Class()),
                 ::com::google::gson::JsonSyntaxException);
}

TEST(GsonFromJsonClassTest, RejectsNullArguments)
{
    const auto gson = ::jxx::NEW<::com::google::gson::Gson>();
    EXPECT_THROW(gson->fromJson(
                     ::jxx::Ptr<::com::google::gson::stream::JsonReader>(),
                     ErasedMap::Class()),
                 ::jxx::lang::NullPointerException);
    EXPECT_THROW(gson->fromJson(reader("{}"), nullptr),
                 ::jxx::lang::NullPointerException);
}
TEST(GsonFromJsonClassTest, RejectsMalformedInput)
{
    const auto gson =
        ::jxx::NEW<::com::google::gson::Gson>();

    EXPECT_THROW(
        gson->fromJson(
            reader("{\"x\":"),
            ErasedMap::Class()),
        ::com::google::gson::JsonSyntaxException);
}