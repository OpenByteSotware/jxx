#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
namespace {
using ::com::google::gson::Gson;
using ::com::google::gson::JsonParser;
static_assert(std::is_base_of_v<::jxx::lang::Object, Gson>);
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonParser>);
TEST(GsonBundles676869TypeTest, PublicTypesRemainObjects) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,Gson>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonParser>));
}
}
