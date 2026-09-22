#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.ToNumberStrategy.h"
#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
namespace { using ::com::google::gson::ToNumberStrategy; using ::com::google::gson::GsonBuilder;
static_assert(std::is_abstract_v<ToNumberStrategy>);static_assert(std::is_base_of_v<::jxx::lang::Object,GsonBuilder>);
TEST(GsonBundles707172TypeTest,TypesRemainCompatible){EXPECT_TRUE((std::is_abstract_v<ToNumberStrategy>));EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,GsonBuilder>));}}
