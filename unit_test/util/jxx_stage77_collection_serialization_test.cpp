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
#include "io/jxx.io.NotSerializableException.h"

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

TEST(Stage77CollectionSerialization, ObjectUpcastAfterRoundTrip)
{
	::jxx::initialize();

	auto bytes =
		::jxx::NEW<::jxx::io::ByteArrayOutputStream>();

	auto out =
		::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);

	out->writeObject(
		::jxx::CAST<::jxx::lang::Object>(
			::jxx::NEW<::jxx::lang::String>("abc")));

	out->flush();

	auto in =
		::jxx::NEW<::jxx::io::ObjectInputStream>(
			::jxx::NEW<::jxx::io::ByteArrayInputStream>(
				bytes->toByteArray()));

	auto object = in->readObject();

	ASSERT_NE(object, nullptr);

	auto stringValue =
		std::dynamic_pointer_cast<
		::jxx::lang::String>(object);

	ASSERT_NE(stringValue, nullptr);

	auto objectValue =
		std::dynamic_pointer_cast<
		::jxx::lang::Object>(object);

	ASSERT_NE(objectValue, nullptr);
}

TEST(Stage77CollectionSerialization,
	IdentityHashMapEmptyRoundTrip)
{
	::jxx::initialize();

	using M =
		::jxx::util::IdentityHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	EXPECT_NO_THROW({
		auto restored = roundTrip(map);
		ASSERT_NE(restored, nullptr);
	});
}

TEST(Stage77CollectionSerialization,
	IdentityHashMapSingleEntryRoundTrip)
{
	::jxx::initialize();

	using M =
		::jxx::util::IdentityHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	map->put(
		::jxx::NEW<::jxx::lang::String>("k"),
		::jxx::NEW<::jxx::lang::String>("v"));

	auto restored = roundTrip(map);

	ASSERT_NE(restored, nullptr);

	EXPECT_EQ(
		restored->size(),
		1);
}

TEST(Stage77CollectionSerialization,
	IdentityHashMapSingleEntryRoundTripDebug)
{
	::jxx::initialize();

	using M =
		::jxx::util::IdentityHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	map->put(
		::jxx::NEW<::jxx::lang::String>("k"),
		::jxx::NEW<::jxx::lang::String>("v"));

	try
	{
		auto restored = roundTrip(map);

		ASSERT_NE(restored, nullptr);

		EXPECT_EQ(restored->size(), 1);
	}
	catch (const std::exception& ex)
	{
		FAIL() << "std::exception: " << ex.what();
	}
	catch (...)
	{
		FAIL() << "non-std exception";
	}
}

TEST(Stage77CollectionSerialization,
	IdentityHashMapWriteOnly)
{
	::jxx::initialize();

	using M =
		::jxx::util::IdentityHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	map->put(
		::jxx::NEW<::jxx::lang::String>("k"),
		::jxx::NEW<::jxx::lang::String>("v"));

	auto bytes =
		::jxx::NEW<::jxx::io::ByteArrayOutputStream>();

	auto out =
		::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);

	EXPECT_NO_THROW(
		out->writeObject(map));

	EXPECT_NO_THROW(
		out->flush());

	EXPECT_GT(bytes->size(), 0);
}

TEST(Stage77CollectionSerialization,
	IdentityHashMapIteratorSanity)
{
	using M =
		::jxx::util::IdentityHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	map->put(
		::jxx::NEW<::jxx::lang::String>("k"),
		::jxx::NEW<::jxx::lang::String>("v"));

	auto set = map->entrySet();

	auto it = set->iterator();

	ASSERT_TRUE(it->hasNext());

	EXPECT_NO_THROW(
		it->next());
}

TEST(Stage77CollectionSerialization,
	WeakHashMapWriteOnly)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	auto bytes =
		::jxx::NEW<::jxx::io::ByteArrayOutputStream>();

	auto out =
		::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);

	EXPECT_NO_THROW(
		out->writeObject(map));

	EXPECT_NO_THROW(
		out->flush());

	EXPECT_GT(
		bytes->size(),
		0);
}

TEST(Stage77CollectionSerialization,
	WeakHashMapWriteOnlySizeCheck)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	EXPECT_EQ(
		map->size(),
		1);
}
TEST(Stage77CollectionSerialization, WeakHashMapIsNotSerializable)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	EXPECT_THROW(
		{
			auto restored = roundTrip(map);
			(void)restored;
		},
		::jxx::io::NotSerializableException);
}
TEST(Stage77Collections, WeakHashMapReadsEntryImmediately)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	const auto map = ::jxx::NEW<M>();

	const auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	const auto equivalentKey =
		::jxx::NEW<::jxx::lang::String>("key");

	EXPECT_EQ(map->size(), 1);
	EXPECT_NE(map->get(key), nullptr);
	EXPECT_NE(map->get(equivalentKey), nullptr);
}
TEST(Stage77CollectionSerialization, WeakHashMapRoundTripDiagnostics)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	auto map = ::jxx::NEW<M>();

	auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	auto restored = roundTrip(map);

	ASSERT_NE(restored, nullptr);

	const auto count = restored->size();

	std::cout
		<< "restored size = "
		<< count
		<< std::endl;

	auto iterator =
		restored->entrySet()->iterator();

	while (iterator->hasNext())
	{
		auto entry = iterator->next();

		auto stringKey =
			std::dynamic_pointer_cast<
			::jxx::lang::String>(
				entry->getKey());

		if (stringKey)
		{
			std::cout
				<< "key="
				<< stringKey->utf8()
				<< std::endl;
		}
	}
}
TEST(Stage77CollectionSerialization, WeakHashMapIsNotSerializable2)
{
	::jxx::initialize();

	using M =
		::jxx::util::WeakHashMap<
		::jxx::lang::Object,
		::jxx::lang::Object>;

	const auto map = ::jxx::NEW<M>();

	const auto key =
		::jxx::NEW<::jxx::lang::String>("key");

	map->put(
		key,
		::jxx::NEW<::jxx::lang::String>("value"));

	const auto bytes =
		::jxx::NEW<::jxx::io::ByteArrayOutputStream>();

	const auto output =
		::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);

	EXPECT_THROW(
		output->writeObject(
			::jxx::CAST<::jxx::lang::Object>(map)),
		::jxx::io::NotSerializableException);
}