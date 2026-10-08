#include <gtest/gtest.h>

#include "util/jxx.util.Date.h"

TEST(JxxDateJava8ParityTest, HierarchyMatchesJava8) {
    EXPECT_TRUE(::jxx::lang::Cloneable::Class()->isAssignableFrom(
        ::jxx::util::Date::Class()));
    EXPECT_TRUE(::jxx::lang::Comparable<::jxx::util::Date>::Class()->isAssignableFrom(
        ::jxx::util::Date::Class()));
    EXPECT_TRUE(::jxx::io::SerializableI::Class()->isAssignableFrom(
        ::jxx::util::Date::Class()));
}

TEST(JxxDateJava8ParityTest, OrderingEqualityAndHashUseEpochMillis) {
    const auto first = ::jxx::NEW<::jxx::util::Date>(10);
    const auto equal = ::jxx::NEW<::jxx::util::Date>(10);
    const auto later = ::jxx::NEW<::jxx::util::Date>(20);

    EXPECT_TRUE(first->before(later));
    EXPECT_TRUE(later->after(first));
    EXPECT_LT(first->compareTo(later), 0);
    EXPECT_EQ(first->compareTo(equal), 0);
    EXPECT_TRUE(first->equals(equal));
    EXPECT_EQ(first->hashCode(), equal->hashCode());
}

TEST(JxxDateJava8ParityTest, NullComparisonArgumentsAreRejected) {
    const auto date = ::jxx::NEW<::jxx::util::Date>(10);
    EXPECT_THROW(date->before(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(date->after(nullptr), ::jxx::lang::NullPointerException);
    EXPECT_THROW(date->compareTo(nullptr), ::jxx::lang::NullPointerException);
}
