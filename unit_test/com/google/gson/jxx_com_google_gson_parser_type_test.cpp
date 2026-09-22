#include <gtest/gtest.h>
#include <type_traits>

#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"

namespace {
using ::com::google::gson::Gson;
using ::com::google::gson::JsonParser;
using ::com::google::gson::JsonSyntaxException;
static_assert(std::is_base_of_v<::jxx::lang::Object, Gson>);
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonParser>);
static_assert(std::is_base_of_v<::jxx::lang::RuntimeException, JsonSyntaxException>);
TEST(GsonParserTypeTest, PublicTypesHaveExpectedRelationships) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, Gson>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, JsonParser>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::RuntimeException, JsonSyntaxException>));
}
} // namespace
