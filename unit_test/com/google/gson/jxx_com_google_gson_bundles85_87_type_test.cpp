#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.GsonBuilder.h"
#include "com/google/gson/jxx.com.google.gson.JsonPrimitive.h"
namespace{using namespace ::com::google::gson;static_assert(std::is_base_of_v<::jxx::lang::Object,Gson>);static_assert(std::is_base_of_v<::jxx::lang::Object,GsonBuilder>);static_assert(std::is_base_of_v<::jxx::lang::Object,JsonPrimitive>);TEST(GsonBundles858687TypeTest,PublicTypesRemainObjects){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,Gson>));}}
