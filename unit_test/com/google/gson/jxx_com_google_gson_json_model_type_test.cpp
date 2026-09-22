#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
namespace {
using namespace ::com::google::gson;
static_assert(std::is_base_of_v<JsonElement,JsonArray>);
static_assert(std::is_base_of_v<JsonElement,JsonNull>);
static_assert(std::is_base_of_v<JsonElement,JsonObject>);
static_assert(std::is_base_of_v<JsonElement,JsonPrimitive>);
TEST(GsonJsonModelTypeTest,PublicHierarchyIsStable){EXPECT_TRUE((std::is_base_of_v<JsonElement,JsonArray>));EXPECT_TRUE((std::is_base_of_v<JsonElement,JsonObject>));}
}
