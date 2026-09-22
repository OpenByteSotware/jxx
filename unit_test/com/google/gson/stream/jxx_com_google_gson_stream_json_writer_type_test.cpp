#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonWriter.h"
namespace {
using ::com::google::gson::stream::JsonWriter;
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonWriter>);
static_assert(std::is_base_of_v<::jxx::io::Closeable, JsonWriter>);
static_assert(std::is_base_of_v<::jxx::io::Flushable, JsonWriter>);
TEST(GsonJsonWriterTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, JsonWriter>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::Closeable, JsonWriter>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::Flushable, JsonWriter>));
}
} // namespace
