#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "util/jxx.util.Properties.h"

TEST(PropertiesStringPropertyNamesParity, SupportsSetQueriesAndHashCode) {
    const auto properties = ::jxx::NEW<::jxx::util::Properties>();
    properties->setProperty(
        ::jxx::NEW<::jxx::lang::String>("alpha"),
        ::jxx::NEW<::jxx::lang::String>("one"));
    properties->setProperty(
        ::jxx::NEW<::jxx::lang::String>("beta"),
        ::jxx::NEW<::jxx::lang::String>("two"));

    const auto first = properties->stringPropertyNames();
    const auto second = properties->stringPropertyNames();
    ASSERT_NE(nullptr, first);
    ASSERT_NE(nullptr, second);
    EXPECT_TRUE(first->equals(::jxx::CAST<::jxx::lang::Object>(second)));
    EXPECT_EQ(first->hashCode(), second->hashCode());
    EXPECT_TRUE(first->containsAll(::jxx::CAST<::jxx::util::wildcard::CollectionAny>(second)));
}

TEST(PropertiesStringPropertyNamesParity, SuppliesDistinctSizedSpliterator) {
    const auto properties = ::jxx::NEW<::jxx::util::Properties>();
    properties->setProperty(
        ::jxx::NEW<::jxx::lang::String>("alpha"),
        ::jxx::NEW<::jxx::lang::String>("one"));

    const auto names = properties->stringPropertyNames();
    const auto spliterator = names->spliterator();
    ASSERT_NE(nullptr, spliterator);
    EXPECT_EQ(1, spliterator->estimateSize());
    EXPECT_NE(0, spliterator->characteristics() &
        ::jxx::util::Spliterator<::jxx::lang::String>::DISTINCT);
    EXPECT_NE(0, spliterator->characteristics() &
        ::jxx::util::Spliterator<::jxx::lang::String>::SIZED);
}

TEST(PropertiesStringPropertyNamesParity, RemainsUnmodifiable) {
    const auto properties = ::jxx::NEW<::jxx::util::Properties>();
    properties->setProperty(
        ::jxx::NEW<::jxx::lang::String>("alpha"),
        ::jxx::NEW<::jxx::lang::String>("one"));
    const auto names = properties->stringPropertyNames();

    EXPECT_THROW(
        names->add(::jxx::NEW<::jxx::lang::String>("beta")),
        ::jxx::lang::UnsupportedOperationException);
}
