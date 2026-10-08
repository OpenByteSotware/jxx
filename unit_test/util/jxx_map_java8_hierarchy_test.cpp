#include <gtest/gtest.h>
#include "util/jxx.util.WeakHashMap.h"
#include "util/jxx.util.IdentityHashMap.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "util/jxx.util.HashMap.h"

TEST(JxxMapJava8HierarchyTest, PublicMapsExtendAbstractMap) {
    using Weak = ::jxx::util::WeakHashMap<::jxx::lang::String, ::jxx::lang::String>;
    using Identity = ::jxx::util::IdentityHashMap<::jxx::lang::String, ::jxx::lang::String>;
    using Abstract = ::jxx::util::AbstractMap<::jxx::lang::String, ::jxx::lang::String>;
    using Hash = ::jxx::util::HashMap<::jxx::lang::String, ::jxx::lang::String>;

    EXPECT_TRUE(Abstract::Class()->isAssignableFrom(Weak::Class()));
    EXPECT_TRUE(Abstract::Class()->isAssignableFrom(Identity::Class()));
    EXPECT_FALSE(Hash::Class()->isAssignableFrom(Weak::Class()));
    EXPECT_FALSE(Hash::Class()->isAssignableFrom(Identity::Class()));
}
