#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.AbstractMap.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "util/jxx.util.Map.h"

using IdentityObjectMap = ::jxx::util::IdentityHashMap<
    ::jxx::lang::Object, ::jxx::lang::Object>;

TEST(IdentityHashMapHierarchyParity, MatchesJava8Interfaces) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::AbstractMap<::jxx::lang::Object, ::jxx::lang::Object>,
        IdentityObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::Map<::jxx::lang::Object, ::jxx::lang::Object>,
        IdentityObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, IdentityObjectMap>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, IdentityObjectMap>));
}
