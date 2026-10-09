#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.Hashtable.h"

namespace {
using Object = ::jxx::lang::Object;
using String = ::jxx::lang::String;

TEST(HashMapValueEqualityParity, EqualStringInstancesAddressSameEntry) {
    const auto map = ::jxx::NEW<::jxx::util::HashMap<String, String>>();
    const auto storedKey = ::jxx::NEW<String>("equivalent-key");
    const auto lookupKey = ::jxx::NEW<String>("equivalent-key");
    const auto value = ::jxx::NEW<String>("mapped-value");
    ASSERT_NE(storedKey.get(), lookupKey.get());
    ASSERT_TRUE(storedKey->equals(::jxx::CAST<Object>(lookupKey)));
    ASSERT_EQ(storedKey->hashCode(), lookupKey->hashCode());
    EXPECT_EQ(nullptr, map->put(storedKey, value));
    const auto mapped = map->get(::jxx::CAST<Object>(lookupKey));
    ASSERT_NE(nullptr, mapped);
    EXPECT_EQ("mapped-value", mapped->utf8());
    EXPECT_TRUE(map->containsKey(::jxx::CAST<Object>(lookupKey)));
    EXPECT_NE(nullptr, map->remove(::jxx::CAST<Object>(lookupKey)));
    EXPECT_TRUE(map->isEmpty());
}

TEST(HashtableValueEqualityParity, EqualStringInstancesAddressSameEntry) {
    const auto table = ::jxx::NEW<::jxx::util::Hashtable<Object, Object>>();
    const auto storedKey = ::jxx::NEW<String>("equivalent-key");
    const auto lookupKey = ::jxx::NEW<String>("equivalent-key");
    const auto value = ::jxx::NEW<String>("mapped-value");
    table->put(::jxx::CAST<Object>(storedKey), ::jxx::CAST<Object>(value));
    const auto mapped = ::jxx::CAST<String>(
        table->get(::jxx::CAST<Object>(lookupKey)));
    ASSERT_NE(nullptr, mapped);
    EXPECT_EQ("mapped-value", mapped->utf8());
}
} // namespace
