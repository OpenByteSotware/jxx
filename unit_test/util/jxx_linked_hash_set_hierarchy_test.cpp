#include <gtest/gtest.h>
#include <type_traits>
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.HashSet.h"
#include "util/jxx.util.LinkedHashSet.h"

using LinkedObjectSet = ::jxx::util::LinkedHashSet<::jxx::lang::Object>;
using ObjectHashSet = ::jxx::util::HashSet<::jxx::lang::Object>;

TEST(LinkedHashSetHierarchyParity, ExtendsHashSet) {
    EXPECT_TRUE((std::is_base_of_v<ObjectHashSet, LinkedObjectSet>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::lang::Cloneable, LinkedObjectSet>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::SerializableI, LinkedObjectSet>));
    EXPECT_FALSE(std::is_abstract_v<LinkedObjectSet>);
}

TEST(LinkedHashSetHierarchyParity, ClonePreservesInsertionOrder) {
    auto source = ::jxx::NEW<LinkedObjectSet>();
    source->add(::jxx::NEW<::jxx::lang::String>("first"));
    source->add(::jxx::NEW<::jxx::lang::String>("second"));
    auto clone = std::dynamic_pointer_cast<LinkedObjectSet>(source->clone());
    ASSERT_NE(clone, nullptr);
    auto iterator = clone->iterator();
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(iterator->next())->utf8(), "first");
    ASSERT_TRUE(iterator->hasNext());
    EXPECT_EQ(std::dynamic_pointer_cast<::jxx::lang::String>(iterator->next())->utf8(), "second");
}
