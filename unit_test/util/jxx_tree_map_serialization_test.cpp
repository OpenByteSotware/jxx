#include <gtest/gtest.h>

#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.TreeMap.h"

TEST(TreeMapSerialization, RoundTripsNaturalOrderEntriesAndNullValue) {
    using String = ::jxx::lang::String;
    using Map = ::jxx::util::TreeMap<String, String>;

    auto original = ::jxx::NEW<Map>();
    original->put(::jxx::NEW<String>("b"), ::jxx::NEW<String>("two"));
    original->put(::jxx::NEW<String>("a"), nullptr);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(original);
    output->close();

    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = ::jxx::CAST<Map>(input->readObject());

    ASSERT_NE(nullptr, restored);
    EXPECT_EQ(2, restored->size());
    EXPECT_TRUE(restored->containsKey(::jxx::NEW<String>("a")));
    EXPECT_EQ(nullptr, restored->get(::jxx::NEW<String>("a")));
    EXPECT_TRUE(restored->get(::jxx::NEW<String>("b"))->equals(
        ::jxx::NEW<String>("two")));
    EXPECT_TRUE(restored->firstKey()->equals(::jxx::NEW<String>("a")));
}
