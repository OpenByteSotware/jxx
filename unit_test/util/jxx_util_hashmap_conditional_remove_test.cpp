#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"

namespace {
TEST(HashMapConditionalRemoveParityTest, TwoArgumentRemoveIsVisibleOnConcreteType) {
    using ::jxx::lang::String;
    using MapType = ::jxx::util::HashMap<String, String>;
    const auto map = ::jxx::NEW<MapType>();
    const auto key = ::jxx::NEW<String>("key");
    const auto value = ::jxx::NEW<String>("value");
    map->put(key, value);
    EXPECT_TRUE(map->remove(
        ::jxx::CAST<::jxx::lang::Object>(key),
        ::jxx::CAST<::jxx::lang::Object>(value)));
}
}
