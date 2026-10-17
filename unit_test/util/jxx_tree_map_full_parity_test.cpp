#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.NavigableMap.h"
#include "util/jxx.util.SortedMap.h"
#include "util/jxx.util.TreeMap.h"

using String = ::jxx::lang::String;
using Map = ::jxx::util::TreeMap<String, String>;

TEST(TreeMapFullParity, ImplementsTransitiveInterfaceHierarchy) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::NavigableMap<String,String>, Map>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::SortedMap<String,String>, Map>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::util::Map<String,String>, Map>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, Map>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, Map>));
    EXPECT_FALSE(std::is_abstract_v<Map>);
}

TEST(TreeMapFullParity, NavigationEntriesAreImmutableSnapshots) {
    auto map=::jxx::NEW<Map>();
    map->put(::jxx::NEW<String>("a"),::jxx::NEW<String>("1"));
    auto entry=map->firstEntry(); ASSERT_NE(entry,nullptr);
    map->put(::jxx::NEW<String>("a"),::jxx::NEW<String>("2"));
    EXPECT_EQ(entry->getValue()->utf8(),"1");
    EXPECT_THROW(entry->setValue(::jxx::NEW<String>("3")),::jxx::lang::UnsupportedOperationException);
}

TEST(TreeMapFullParity, CloneIsConcreteShallowCopy) {
    auto map=::jxx::NEW<Map>();
    auto key=::jxx::NEW<String>("a"); auto value=::jxx::NEW<String>("1");
    map->put(key,value);
    auto copy=std::dynamic_pointer_cast<Map>(map->clone());
    ASSERT_NE(copy,nullptr); EXPECT_NE(copy.get(),map.get());
    EXPECT_EQ(copy->get(key).get(),value.get());
}
