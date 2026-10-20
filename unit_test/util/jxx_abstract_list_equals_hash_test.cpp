#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.HashMap.h"
#include "util/jxx.util.List.h"

TEST(AbstractListParity, EqualListsHaveEqualHashCodesAndWorkAsMapKeys)
{
    using Object = ::jxx::lang::Object;
    using Value = ::jxx::lang::String;
    auto first = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::NEW<Value>("button")),
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)));
    auto second = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::NEW<Value>("button")),
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)));

    auto firstObject = ::jxx::CAST<Object>(first);
    auto secondObject = ::jxx::CAST<Object>(second);
    ASSERT_NE(firstObject, nullptr);
    ASSERT_NE(secondObject, nullptr);
    EXPECT_TRUE(firstObject->equals(secondObject));
    EXPECT_EQ(firstObject->hashCode(), secondObject->hashCode());

    auto map = ::jxx::NEW<::jxx::util::HashMap<
        ::jxx::util::List<Object>, Value>>();
    map->put(first, ::jxx::NEW<Value>("cached"));
    auto found = map->get(secondObject);
    ASSERT_NE(found, nullptr);
    EXPECT_EQ(found->utf8(), "cached");
}

TEST(AbstractListParity, OrderAndElementValuesParticipateInEquality)
{
    using Object = ::jxx::lang::Object;
    auto first = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::NEW<::jxx::lang::String>("button")),
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)));
    auto reversed = ::jxx::util::Arrays::asList<Object>(
        ::jxx::CAST<Object>(::jxx::lang::Integer::valueOf(3)),
        ::jxx::CAST<Object>(::jxx::NEW<::jxx::lang::String>("button")));
    EXPECT_FALSE(::jxx::CAST<Object>(first)->equals(
        ::jxx::CAST<Object>(reversed)));
}
