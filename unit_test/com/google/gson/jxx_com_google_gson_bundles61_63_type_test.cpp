#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/internal/jxx.com.google.gson.internal.Streams.h"
#include "com/google/gson/jxx.com.google.gson.JsonStreamParser.h"
#include "com/google/gson/jxx.com.google.gson.LongSerializationPolicy.h"
namespace {
using ::com::google::gson::JsonStreamParser;
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonStreamParser>);
TEST(GsonBundles616263TypeTest, StreamParserIsObject) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, JsonStreamParser>));
}
} // namespace
