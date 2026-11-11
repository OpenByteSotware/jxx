#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Map.h"
#include "util/jxx.util.MapEntry.h"

TEST(MapEntryNestedTypeParity, ExposesJava8MapEntryName) {
    using Key = ::jxx::lang::String;
    using Value = ::jxx::lang::Integer;
    using NestedEntry = ::jxx::util::Map<Key, Value>::Entry;
    using ExistingEntry = ::jxx::util::MapEntry<Key, Value>;

    EXPECT_TRUE((std::is_same_v<NestedEntry, ExistingEntry>));
}
