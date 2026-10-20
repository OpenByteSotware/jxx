#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/concurrent/jxx.util.concurrent.ConcurrentHashMap.h"

namespace {

using Map = ::jxx::util::concurrent::ConcurrentHashMap<
    ::jxx::lang::Integer, ::jxx::lang::String>;

TEST(ConcurrentHashMapSerializationParity, ExplicitHooksRoundTripEntries)
{
    auto source = ::jxx::NEW<Map>();
    source->put(::jxx::lang::Integer::valueOf(1),
        ::jxx::NEW<::jxx::lang::String>("one"));
    source->put(::jxx::lang::Integer::valueOf(2),
        ::jxx::NEW<::jxx::lang::String>("two"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));
    source->writeObject(output);
    output->flush();

    auto inputBytes = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(
        bytes->toByteArray());
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::CAST<::jxx::io::InputStream>(inputBytes));
    auto restored = ::jxx::NEW<Map>();
    restored->readObject(input);

    EXPECT_EQ(restored->size(), 2);
    auto one = restored->get(::jxx::lang::Integer::valueOf(1));
    auto two = restored->get(::jxx::lang::Integer::valueOf(2));
    ASSERT_NE(one, nullptr);
    ASSERT_NE(two, nullptr);
    EXPECT_EQ(one->utf8(), "one");
    EXPECT_EQ(two->utf8(), "two");
}

TEST(ConcurrentHashMapSerializationParity, ReadObjectNoDataProducesEmptyMap)
{
    auto map = ::jxx::NEW<Map>();
    map->put(::jxx::lang::Integer::valueOf(1),
        ::jxx::NEW<::jxx::lang::String>("one"));
    map->readObjectNoData();
    EXPECT_TRUE(map->isEmpty());
}

} // namespace
