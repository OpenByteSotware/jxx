#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/jxx.com.google.gson.JsonParseException.h"
#include "com/google/gson/jxx.com.google.gson.JsonSyntaxException.h"
namespace{using namespace ::com::google::gson;static_assert(std::is_base_of_v<JsonParseException,JsonSyntaxException>);TEST(GsonBundles737475TypeTest,ExceptionHierarchy){EXPECT_TRUE((std::is_base_of_v<JsonParseException,JsonSyntaxException>));}}
