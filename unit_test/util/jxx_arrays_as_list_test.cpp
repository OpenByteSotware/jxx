#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashMap.h"

namespace {

TEST(ArraysAsListParity, SupportsSourceStyleObjectArguments)
{
    auto cacheId = ::jxx::NEW<::jxx::lang::String>("fullscreen");
    auto id = ::jxx::lang::Integer::valueOf(7);
    auto key = ::jxx::util::Arrays::asList<::jxx::lang::Object>(
        ::jxx::CAST<::jxx::lang::Object>(cacheId),
        ::jxx::CAST<::jxx::lang::Object>(id));

    ASSERT_NE(key, nullptr);
    EXPECT_EQ(key->size(), 2);
    auto first = ::jxx::CAST<::jxx::lang::String>(key->get(0));
    auto second = ::jxx::CAST<::jxx::lang::Integer>(key->get(1));
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_EQ(first->utf8(), "fullscreen");
    EXPECT_EQ(second->intValue(), 7);
}

TEST(ArraysAsListParity, EquivalentListsWorkAsMapKeys)
{
    using Object = ::jxx::lang::Object;
    using Value = ::jxx::lang::String;
    auto first = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::NEW<::jxx::lang::String>("button")),
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)));
    auto second = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::NEW<::jxx::lang::String>("button")),
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)));

    auto map = ::jxx::NEW<::jxx::util::HashMap<
        ::jxx::util::List<Object>, Value>>();
    map->put(first, ::jxx::NEW<Value>("cached"));
    auto found = map->get(::jxx::CAST<Object>(second));
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->utf8(), "cached");
}

} // namespace
