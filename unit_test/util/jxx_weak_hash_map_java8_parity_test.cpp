#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.NotSerializableException.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "io/jxx.io.SerializableI.h"
#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.WeakHashMap.h"

using WeakObjectMap =
    ::jxx::util::WeakHashMap<::jxx::lang::Object, ::jxx::lang::Object>;

static_assert(std::is_base_of_v<
    ::jxx::util::AbstractMap<::jxx::lang::Object, ::jxx::lang::Object>,
    WeakObjectMap>);
static_assert(std::is_base_of_v<
    ::jxx::util::Map<::jxx::lang::Object, ::jxx::lang::Object>,
    WeakObjectMap>);
static_assert(!std::is_base_of_v<::jxx::io::SerializableI, WeakObjectMap>);

TEST(WeakHashMapJava8Parity, DoesNotImplementSerializableI) {
    EXPECT_FALSE((std::is_base_of_v<::jxx::io::SerializableI, WeakObjectMap>));
}

TEST(WeakHashMapJava8Parity, ObjectOutputStreamRejectsSerialization) {
    ::jxx::initialize();
    auto map = ::jxx::NEW<WeakObjectMap>();
    auto key = ::jxx::NEW<::jxx::lang::String>("key");
    map->put(key, ::jxx::NEW<::jxx::lang::String>("value"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto output = ::jxx::NEW<::jxx::io::ObjectOutputStream>(bytes);

    EXPECT_THROW(output->writeObject(map), ::jxx::io::NotSerializableException);
}
