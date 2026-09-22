#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.JsonParser.h"
#include "com/google/gson/reflect/jxx.com.google.gson.reflect.TypeToken.h"
namespace{using ::com::google::gson::JsonParser;using ::com::google::gson::reflect::TypeToken;static_assert(std::is_base_of_v<::jxx::lang::Object,JsonParser>);static_assert(std::is_base_of_v<::jxx::lang::Object,TypeToken>);TEST(GsonBundles979899TypeTest,TypesRemainObjects){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonParser>));}}
