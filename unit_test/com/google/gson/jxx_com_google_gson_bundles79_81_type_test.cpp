#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/internal/jxx.com.google.gson.internal.GsonContext.h"
namespace{using ::com::google::gson::internal::GsonContext;static_assert(std::is_base_of_v<::jxx::lang::Object,GsonContext>);static_assert(std::is_base_of_v<::com::google::gson::JsonSerializationContext,GsonContext>);static_assert(std::is_base_of_v<::com::google::gson::JsonDeserializationContext,GsonContext>);TEST(GsonBundles798081TypeTest,ContextRelationships){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,GsonContext>));}}
