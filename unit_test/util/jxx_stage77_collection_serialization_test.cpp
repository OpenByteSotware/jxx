#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "util/jxx.util.LinkedHashMap.h"
#include "util/jxx.util.WeakHashMap.h"

using Object = ::jxx::lang::Object;
template<typename T> ::jxx::Ptr<T> roundTrip(const ::jxx::Ptr<T>& source)
{
	auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>(); 
	auto out = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes); out->writeObject(source); out->flush(); auto in = ::jxx::NEW<::jxx::io::ObjectInputStream>(::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray())); return std::dynamic_pointer_cast<T>(in->readObject());
}

TEST(Stage77CollectionSerialization, IdentityHashMapPreservesDistinctEqualKeys)
{
	::jxx::initialize(); 
	using M = ::jxx::util::IdentityHashMap<Object, Object>;
	auto map = ::jxx::NEW<M>();
	auto first = ::jxx::NEW<::jxx::lang::String>("same"); 
	auto second = ::jxx::NEW<::jxx::lang::String>("same");
	map->put(first, ::jxx::NEW<::jxx::lang::String>("one")); 
	map->put(second, ::jxx::NEW<::jxx::lang::String>("two"));
	auto restored = roundTrip(map); ASSERT_NE(restored, nullptr); 
	EXPECT_EQ(restored->size(), 2);
}

TEST(Stage77CollectionSerialization, LinkedHashMapPreservesEncounterOrder)
{
	::jxx::initialize(); 
	using M = ::jxx::util::LinkedHashMap<Object, Object>; 
	auto map = ::jxx::NEW<M>();
	map->put(::jxx::NEW<::jxx::lang::String>("first"),
		::jxx::NEW<::jxx::lang::String>("1")); 
	map->put(::jxx::NEW<::jxx::lang::String>("second"), 
		::jxx::NEW<::jxx::lang::String>("2")); 
	auto restored = roundTrip(map); 
	ASSERT_NE(restored, nullptr); 
	auto it = restored->entrySet()->iterator(); 
	ASSERT_TRUE(it->hasNext()); 
	EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(it->next()->getKey())->utf8(), "first");
}

TEST(Stage77CollectionSerialization, WeakHashMapRoundTripsLiveEntries)
{
	::jxx::initialize(); 
	using M = ::jxx::util::WeakHashMap<Object, Object>; 
	auto map = ::jxx::NEW<M>(); auto key = ::jxx::NEW<::jxx::lang::String>("key"); 
	map->put(key, ::jxx::NEW<::jxx::lang::String>("value"));
	auto restored = roundTrip(map); ASSERT_NE(restored, nullptr);
	EXPECT_EQ(restored->size(), 1);
}
