#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
namespace {using ::com::google::gson::stream::JsonReader;using ::com::google::gson::stream::JsonWriter;using ::com::google::gson::JsonPrimitive;static_assert(std::is_base_of_v<::jxx::lang::Object,JsonReader>);static_assert(std::is_base_of_v<::jxx::lang::Object,JsonWriter>);static_assert(std::is_base_of_v<::jxx::lang::Object,JsonPrimitive>);TEST(GsonBundles828384TypeTest,TypesRemainObjects){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonReader>));}}
