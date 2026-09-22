#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Expose.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.JsonAdapter.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.SerializedName.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Since.h"
#include "com/google/gson/annotations/jxx.com.google.gson.annotations.Until.h"
#include "com/google/gson/jxx.com.google.gson.ExclusionStrategy.h"
#include "com/google/gson/jxx.com.google.gson.FieldAttributes.h"
namespace {
using namespace ::com::google::gson;
using namespace ::com::google::gson::annotations;
static_assert(std::is_base_of_v<::jxx::lang::Object, SerializedName>);
static_assert(std::is_base_of_v<::jxx::lang::Object, Expose>);
static_assert(std::is_base_of_v<::jxx::lang::Object, Since>);
static_assert(std::is_base_of_v<::jxx::lang::Object, Until>);
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonAdapter>);
static_assert(std::is_abstract_v<ExclusionStrategy>);
static_assert(std::is_base_of_v<::jxx::lang::Object, FieldAttributes>);
TEST(GsonAnnotationsExclusionTypeTest, PublicRelationshipsAreStable) {
 EXPECT_TRUE((std::is_abstract_v<ExclusionStrategy>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, SerializedName>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, FieldAttributes>));
}
} // namespace
