#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.JsonArray.h"
#include "com/google/gson/jxx.com.google.gson.JsonNull.h"
#include "com/google/gson/jxx.com.google.gson.JsonObject.h"
namespace{using namespace ::com::google::gson;static_assert(std::is_base_of_v<::jxx::lang::Object,JsonNull>);static_assert(std::is_base_of_v<::jxx::lang::Object,JsonArray>);static_assert(std::is_base_of_v<::jxx::lang::Object,JsonObject>);TEST(GsonBundles919293TypeTest,TreeTypesRemainObjects){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonNull>));}}
