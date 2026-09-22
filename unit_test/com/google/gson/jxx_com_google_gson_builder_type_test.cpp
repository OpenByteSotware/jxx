#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
namespace {
using ::com::google::gson::Gson;
using ::com::google::gson::GsonBuilder;
static_assert(std::is_base_of_v<::jxx::lang::Object, Gson>);
static_assert(std::is_base_of_v<::jxx::lang::Object, GsonBuilder>);
TEST(GsonBuilderTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, Gson>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, GsonBuilder>));
}
} // namespace
