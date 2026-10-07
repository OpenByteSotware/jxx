#include <gtest/gtest.h>
#include "util/jxx.util.Stack.h"
#include "util/jxx.util.WeakHashMap.h"
#include "util/jxx.util.IdentityHashMap.h"

TEST(JxxCollectionHierarchyTest, StackRetainsVectorBehavior) {
    const auto stack = ::jxx::NEW<::jxx::util::Stack<::jxx::lang::String>>();
    const auto value = ::jxx::NEW<::jxx::lang::String>("value");
    stack->push(value);
    EXPECT_EQ(stack->peek(), value);
    EXPECT_EQ(stack->pop(), value);
    EXPECT_TRUE(stack->empty());
}

TEST(JxxCollectionHierarchyTest, MapSubclassesExposeDistinctClassMetadata) {
    EXPECT_NE((::jxx::util::WeakHashMap<::jxx::lang::String, ::jxx::lang::String>::Class()),
              (::jxx::util::IdentityHashMap<::jxx::lang::String, ::jxx::lang::String>::Class()));
}
