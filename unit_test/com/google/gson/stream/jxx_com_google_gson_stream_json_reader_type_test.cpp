#include <gtest/gtest.h>
#include <type_traits>
#include "com/google/gson/stream/jxx.com.google.gson.stream.JsonReader.h"
#include "com/google/gson/stream/jxx.com.google.gson.stream.MalformedJsonException.h"
namespace {
using ::com::google::gson::stream::JsonReader;
using ::com::google::gson::stream::MalformedJsonException;
static_assert(std::is_base_of_v<::jxx::lang::Object, JsonReader>);
static_assert(std::is_base_of_v<::jxx::io::IOException, MalformedJsonException>);
TEST(GsonJsonReaderTypeTest, PublicRelationshipsAreStable) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Object, JsonReader>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::IOException, MalformedJsonException>));
}
} // namespace
