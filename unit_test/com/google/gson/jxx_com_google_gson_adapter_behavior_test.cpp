#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.Gson.h"
#include "com/google/gson/jxx.com.google.gson.TypeAdapter.h"
namespace {
using ::com::google::gson::Gson;
using ::com::google::gson::TypeAdapter;
static_assert(std::is_abstract_v<TypeAdapter>);
static_assert(std::is_base_of_v<::jxx::lang::Object, Gson>);
TEST(GsonAdapterBehaviorTest, AdapterContractIsAvailable) {
    EXPECT_TRUE((std::is_abstract_v<TypeAdapter>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, Gson>));
}
} // namespace
