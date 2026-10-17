#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.SortedMap.h"
#include "util/jxx.util.TreeMap.h"

using String = ::jxx::lang::String;
using StringTreeMap = ::jxx::util::TreeMap<String, String>;

TEST(TreeMapNavigableMapHierarchy, MatchesJava8Interfaces) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::NavigableMap<String, String>, StringTreeMap>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::SortedMap<String, String>, StringTreeMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, StringTreeMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, StringTreeMap>));
    EXPECT_FALSE(std::is_abstract_v<StringTreeMap>);
}

TEST(TreeMapNavigableMapBehavior, NavigationMethodsUseSortedKeys) {
    auto map = ::jxx::NEW<StringTreeMap>();
    map->put(::jxx::NEW<String>("b"), ::jxx::NEW<String>("2"));
    map->put(::jxx::NEW<String>("a"), ::jxx::NEW<String>("1"));
    map->put(::jxx::NEW<String>("c"), ::jxx::NEW<String>("3"));

    EXPECT_EQ(map->firstKey()->utf8(), "a");
    EXPECT_EQ(map->lastKey()->utf8(), "c");
    EXPECT_EQ(map->lowerKey(::jxx::NEW<String>("b"))->utf8(), "a");
    EXPECT_EQ(map->floorKey(::jxx::NEW<String>("b"))->utf8(), "b");
    EXPECT_EQ(map->ceilingKey(::jxx::NEW<String>("b"))->utf8(), "b");
    EXPECT_EQ(map->higherKey(::jxx::NEW<String>("b"))->utf8(), "c");
}

TEST(TreeMapNavigableMapBehavior, InclusiveAndExclusiveRangeOverloads) {
    auto map = ::jxx::NEW<StringTreeMap>();
    map->put(::jxx::NEW<String>("a"), ::jxx::NEW<String>("1"));
    map->put(::jxx::NEW<String>("b"), ::jxx::NEW<String>("2"));
    map->put(::jxx::NEW<String>("c"), ::jxx::NEW<String>("3"));

    auto range = map->subMap(
        ::jxx::NEW<String>("a"), false,
        ::jxx::NEW<String>("c"), false);
    ASSERT_NE(range, nullptr);
    EXPECT_EQ(range->size(), 1);
    EXPECT_NE(range->get(::jxx::NEW<String>("b")), nullptr);

    auto head = map->headMap(::jxx::NEW<String>("b"), true);
    ASSERT_NE(head, nullptr);
    EXPECT_EQ(head->size(), 2);

    auto tail = map->tailMap(::jxx::NEW<String>("b"), false);
    ASSERT_NE(tail, nullptr);
    EXPECT_EQ(tail->size(), 1);
}
