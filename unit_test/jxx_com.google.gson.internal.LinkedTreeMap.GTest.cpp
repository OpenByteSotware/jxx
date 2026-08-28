#include <gtest/gtest.h>

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "lang/jxx.lang.String.h"

namespace {

using String = jxx::lang::String;
using InnerMap =
    com::google::gson::internal::LinkedTreeMap<String, String>;
using OuterMap =
    com::google::gson::internal::LinkedTreeMap<String, InnerMap>;

TEST(LinkedTreeMapTest, PutAndGetStringValue) {
    auto map = jxx::NEW<InnerMap>();
    auto key = jxx::NEW<String>("name");
    auto value = jxx::NEW<String>("Controller");

    EXPECT_EQ(map->put(key, value), nullptr);
    EXPECT_EQ(map->get(key), value);
    EXPECT_TRUE(map->containsKey(key));
}

TEST(LinkedTreeMapTest, ReplacingValueReturnsPreviousValue) {
    auto map = jxx::NEW<InnerMap>();
    auto key = jxx::NEW<String>("name");
    auto first = jxx::NEW<String>("Controller");
    auto second = jxx::NEW<String>("Display");

    EXPECT_EQ(map->put(key, first), nullptr);
    EXPECT_EQ(map->put(key, second), first);
    EXPECT_EQ(map->get(key), second);
}

TEST(LinkedTreeMapTest, NullKeyThrows) {
    auto map = jxx::NEW<InnerMap>();
    auto value = jxx::NEW<String>("value");

    EXPECT_THROW(
        map->put(nullptr, value),
        jxx::lang::NullPointerException);
}

TEST(LinkedTreeMapTest, SupportsNestedMapRequiredByGson) {
    auto outer = jxx::NEW<OuterMap>();
    auto inner = jxx::NEW<InnerMap>();

    inner->put(
        jxx::NEW<String>("name"),
        jxx::NEW<String>("Controller"));

    outer->put(
        jxx::NEW<String>("device1"),
        inner);

    auto loaded = outer->get(
        jxx::NEW<String>("device1"));

    ASSERT_NE(loaded, nullptr);

    auto name = loaded->get(
        jxx::NEW<String>("name"));

    ASSERT_NE(name, nullptr);
    EXPECT_EQ(name->utf8(), "Controller");
}

TEST(LinkedTreeMapTest, ConcreteSpecializationsHaveDistinctClassDescriptors) {
    EXPECT_NE(InnerMap::Class(), OuterMap::Class());
    EXPECT_FALSE(InnerMap::Class()->isInterface());
    EXPECT_FALSE(OuterMap::Class()->isInterface());
}

} // namespace
