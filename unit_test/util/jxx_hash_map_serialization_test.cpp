#include <gtest/gtest.h>
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashMap.h"

using ErasedHashMap = ::jxx::util::HashMap<::jxx::lang::Object, ::jxx::lang::Object>;

TEST(HashMapSerialization, RoundTripsEntriesAndNullValue) {
    ::jxx::initialize();
    auto source = ::jxx::NEW<ErasedHashMap>();
    auto key = ::jxx::NEW<::jxx::lang::String>("key");
    source->put(key, ::jxx::NEW<::jxx::lang::String>("value"));
    source->put(::jxx::NEW<::jxx::lang::String>("nullable"), nullptr);
    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);
    output->writeObject(source); output->flush();
    auto input = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray()));
    auto restored = std::dynamic_pointer_cast<ErasedHashMap>(input->readObject());
    ASSERT_NE(restored, nullptr); EXPECT_EQ(restored->size(), 2);
    auto value = std::dynamic_pointer_cast<::jxx::lang::String>(restored->get(key));
    ASSERT_NE(value, nullptr); EXPECT_EQ(value->utf8(), "value");
}
