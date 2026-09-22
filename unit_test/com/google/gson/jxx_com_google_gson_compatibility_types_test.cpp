#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.FieldNamingStrategy.h"
#include "com/google/gson/jxx.com.google.gson.InstanceCreator.h"
#include "com/google/gson/jxx.com.google.gson.JsonIOException.h"
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapterFactory.h"
namespace {
using namespace ::com::google::gson;
static_assert(std::is_abstract_v<FieldNamingStrategy>);
static_assert(std::is_abstract_v<InstanceCreator>);
static_assert(std::is_abstract_v<TypeAdapterFactory>);
static_assert(std::is_base_of_v<JsonParseException, JsonIOException>);
TEST(GsonCompatibilityTypesTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_abstract_v<FieldNamingStrategy>));
    EXPECT_TRUE((std::is_abstract_v<InstanceCreator>));
    EXPECT_TRUE((std::is_abstract_v<TypeAdapterFactory>));
    EXPECT_TRUE((std::is_base_of_v<JsonParseException, JsonIOException>));
}
} // namespace
