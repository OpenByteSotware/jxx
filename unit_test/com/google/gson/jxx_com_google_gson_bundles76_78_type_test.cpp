#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/internal/jxx.com.google.gson.internal.TreeTypeAdapter.h"
namespace{using ::com::google::gson::internal::TreeTypeAdapter;static_assert(std::is_base_of_v<::jxx::lang::Object,TreeTypeAdapter>);TEST(GsonBundles767778TypeTest,TreeAdapterIsObject){EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object,TreeTypeAdapter>));}}
