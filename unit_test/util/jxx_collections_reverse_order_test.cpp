#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Collections.h"

TEST(CollectionsReverseOrderTest, ReversesNaturalStringOrdering)
{
    auto values = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>>(3U);
    (*values)[0] = ::jxx::NEW<::jxx::lang::String>("b");
    (*values)[1] = ::jxx::NEW<::jxx::lang::String>("a");
    (*values)[2] = ::jxx::NEW<::jxx::lang::String>("c");

    ::jxx::util::Arrays::sort(
        values,
        ::jxx::util::Collections::reverseOrder<
            ::jxx::lang::String>());

    EXPECT_TRUE((*values)[0]->equals(
        ::jxx::NEW<::jxx::lang::String>("c")));
    EXPECT_TRUE((*values)[1]->equals(
        ::jxx::NEW<::jxx::lang::String>("b")));
    EXPECT_TRUE((*values)[2]->equals(
        ::jxx::NEW<::jxx::lang::String>("a")));
}

TEST(CollectionsReverseOrderTest, ReturnsStableComparatorInstance)
{
    const auto first = ::jxx::util::Collections::reverseOrder<
        ::jxx::lang::String>();
    const auto second = ::jxx::util::Collections::reverseOrder<
        ::jxx::lang::String>();
    EXPECT_EQ(first.get(), second.get());
}
