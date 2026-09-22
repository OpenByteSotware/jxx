#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeReader.h"
#include "com/google/gson/internal/jxx.com.google.gson.internal.JsonTreeWriter.h"
namespace { using ::com::google::gson::internal::JsonTreeReader; using ::com::google::gson::internal::JsonTreeWriter;
static_assert(std::is_base_of_v<::jxx::lang::Object,JsonTreeReader>); static_assert(std::is_base_of_v<::jxx::lang::Object,JsonTreeWriter>);
TEST(GsonBundles646566TypeTest,TreeBridgesAreObjects){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonTreeReader>));EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,JsonTreeWriter>));}}
