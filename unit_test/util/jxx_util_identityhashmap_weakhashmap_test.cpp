#include <gtest/gtest.h>
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "util/jxx.util.WeakHashMap.h"
namespace { using S=::jxx::lang::String;
TEST(IdentityHashMapParityTest, DistinctEqualKeysRemainDistinct){auto map=::jxx::NEW<::jxx::util::IdentityHashMap<S,S>>();auto a=::jxx::NEW<S>("same");auto b=::jxx::NEW<S>("same");map->put(a,::jxx::NEW<S>("a"));map->put(b,::jxx::NEW<S>("b"));EXPECT_EQ(2,map->size());EXPECT_TRUE(map->get(::jxx::CAST<::jxx::lang::Object>(a))->utf8()=="a");EXPECT_TRUE(map->containsValue(::jxx::CAST<::jxx::lang::Object>(map->get(::jxx::CAST<::jxx::lang::Object>(a)))));EXPECT_EQ(2,map->entrySet()->size());}
TEST(IdentityHashMapParityTest, ValuesUseIdentityForContainsValue){auto map=::jxx::NEW<::jxx::util::IdentityHashMap<S,S>>();auto value=::jxx::NEW<S>("v");map->put(::jxx::NEW<S>("k"),value);EXPECT_TRUE(map->containsValue(::jxx::CAST<::jxx::lang::Object>(value)));EXPECT_FALSE(map->containsValue(::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<S>("v"))));}
TEST(WeakHashMapParityTest, ExpiredKeysAreExpungedOnAccess){auto map=::jxx::NEW<::jxx::util::WeakHashMap<S,S>>();{auto key=::jxx::NEW<S>("temporary");map->put(key,::jxx::NEW<S>("value"));EXPECT_EQ(1,map->size());}EXPECT_EQ(0,map->size());EXPECT_TRUE(map->isEmpty());}
TEST(WeakHashMapParityTest, NullKeyIsStronglyRepresented){auto map=::jxx::NEW<::jxx::util::WeakHashMap<S,S>>();auto value=::jxx::NEW<S>("null-value");map->put(nullptr,value);EXPECT_TRUE(map->containsKey(nullptr));EXPECT_EQ(value.get(),map->get(nullptr).get());EXPECT_EQ(1,map->entrySet()->size());}
}
