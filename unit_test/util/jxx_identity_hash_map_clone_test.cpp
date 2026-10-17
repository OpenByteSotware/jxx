#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.IdentityHashMap.h"

using Object = ::jxx::lang::Object;
using IdentityMap = ::jxx::util::IdentityHashMap<Object, Object>;

TEST(IdentityHashMapCloneParity, ReturnsIdentityHashMapShallowCopy) {
    auto source = ::jxx::NEW<IdentityMap>();
    auto key = ::jxx::NEW<::jxx::lang::String>("key");
    auto value = ::jxx::NEW<::jxx::lang::String>("value");
    source->put(key, value);

    auto copy = std::dynamic_pointer_cast<IdentityMap>(source->clone());
    ASSERT_NE(copy, nullptr);
    EXPECT_NE(copy.get(), source.get());
    EXPECT_EQ(copy->size(), 1);
    EXPECT_EQ(copy->get(key).get(), value.get());
}

TEST(IdentityHashMapCloneParity, PreservesDistinctEqualKeys) {
    auto source = ::jxx::NEW<IdentityMap>();
    auto first = ::jxx::NEW<::jxx::lang::String>("same");
    auto second = ::jxx::NEW<::jxx::lang::String>("same");
    source->put(first, ::jxx::NEW<::jxx::lang::String>("one"));
    source->put(second, ::jxx::NEW<::jxx::lang::String>("two"));

    auto copy = std::dynamic_pointer_cast<IdentityMap>(source->clone());
    ASSERT_NE(copy, nullptr);
    EXPECT_EQ(copy->size(), 2);
    EXPECT_NE(copy->get(first), nullptr);
    EXPECT_NE(copy->get(second), nullptr);
}
