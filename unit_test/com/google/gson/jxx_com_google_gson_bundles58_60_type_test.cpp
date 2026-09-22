#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.JsonDeserializer.h"
#include "com/google/gson/jxx.com.google.gson.JsonSerializer.h"
#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"
namespace {
using ::com::google::gson::JsonDeserializer;
using ::com::google::gson::JsonSerializer;
using ::com::google::gson::reflect::TypeToken;
static_assert(std::is_abstract_v<JsonDeserializer>);
static_assert(std::is_abstract_v<JsonSerializer>);
static_assert(std::is_base_of_v<::jxx::lang::Object, TypeToken>);
TEST(GsonBundles585960TypeTest, PublicRelationshipsAreStable) {
 EXPECT_TRUE((std::is_abstract_v<JsonDeserializer>));
 EXPECT_TRUE((std::is_abstract_v<JsonSerializer>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, TypeToken>));
}
} // namespace
