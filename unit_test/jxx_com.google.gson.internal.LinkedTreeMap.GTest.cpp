#include <gtest/gtest.h>
#include <type_traits>

#include "com/google/gson/internal/jxx.com.google.gson.internal.LinkedTreeMap.h"
#include "lang/jxx.lang.String.h"

namespace {

using String = jxx::lang::String;
using InnerMap =
    com::google::gson::internal::LinkedTreeMap<String, String>;
using OuterMap =
    com::google::gson::internal::LinkedTreeMap<String, InnerMap>;

static jxx::Ptr<String> S(const char* value) {
    return jxx::NEW<String>(value);
}

static_assert(!std::is_abstract_v<InnerMap>);
static_assert(std::is_default_constructible_v<InnerMap>);
static_assert(!std::is_abstract_v<OuterMap>);
static_assert(std::is_default_constructible_v<OuterMap>);

TEST(LinkedTreeMapTest, PutAndGetStringValue) {
    auto map = jxx::NEW<InnerMap>();
    auto key = S("name");
    auto value = S("Controller");
    EXPECT_EQ(map->put(key, value), nullptr);
    EXPECT_EQ(map->get(key), value);
    EXPECT_TRUE(map->containsKey(key));
}

TEST(LinkedTreeMapTest, ReplacingValueReturnsPreviousValue) {
    auto map = jxx::NEW<InnerMap>();
    auto key = S("name");
    auto first = S("Controller");
    auto second = S("Display");
    EXPECT_EQ(map->put(key, first), nullptr);
    EXPECT_EQ(map->put(key, second), first);
    EXPECT_EQ(map->get(key), second);
}

TEST(LinkedTreeMapTest, NullKeyThrows) {
    auto map = jxx::NEW<InnerMap>();
    auto value = S("value");
    jxx::Ptr<String> nullKey = nullptr;
    EXPECT_THROW(map->put(nullKey, value), jxx::lang::NullPointerException);
}

TEST(LinkedTreeMapTest, SupportsNestedMapRequiredByGson) {
    auto outer = jxx::NEW<OuterMap>();
    auto inner = jxx::NEW<InnerMap>();
    inner->put(S("name"), S("Controller"));
    outer->put(S("device1"), inner);
    auto loaded = outer->get(S("device1"));
    ASSERT_NE(loaded, nullptr);
    auto name = loaded->get(S("name"));
    ASSERT_NE(name, nullptr);
    EXPECT_EQ(name->utf8(), "Controller");
}

TEST(LinkedTreeMapTest, EntryViewRetainsMapOwner) {
    auto map = jxx::NEW<InnerMap>();
    map->put(S("name"), S("Controller"));
    auto iterator = map->entrySet()->iterator();
    ASSERT_TRUE(iterator->hasNext());
    auto entry = iterator->next();
    ASSERT_NE(entry, nullptr);
    map.reset();
    ASSERT_NE(entry->getKey(), nullptr);
    ASSERT_NE(entry->getValue(), nullptr);
    EXPECT_EQ(entry->getKey()->utf8(), "name");
    EXPECT_EQ(entry->getValue()->utf8(), "Controller");
}

TEST(LinkedTreeMapTest, ConcreteSpecializationsHaveDistinctClassDescriptors) {
    auto innerDescriptor = InnerMap::Class();
    auto outerDescriptor = OuterMap::Class();
    ASSERT_NE(innerDescriptor, nullptr);
    ASSERT_NE(outerDescriptor, nullptr);
    EXPECT_NE(innerDescriptor, outerDescriptor);
    EXPECT_FALSE(innerDescriptor->isInterface());
    EXPECT_FALSE(outerDescriptor->isInterface());
}

} // namespace
