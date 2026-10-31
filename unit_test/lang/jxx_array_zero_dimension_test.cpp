#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.JxxArray.h"

TEST(JxxArrayParity, RankOneAllowsZeroLength)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1>>(0);
    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->size(), 0U);
    EXPECT_EQ(values->shape()[0], 0U);
    EXPECT_EQ(values->data(), nullptr);
}

TEST(JxxArrayParity, HigherRankAllowsZeroDimension)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 2>>(3, 0);
    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->size(), 0U);
    EXPECT_EQ(values->shape()[0], 3U);
    EXPECT_EQ(values->shape()[1], 0U);
    EXPECT_EQ(values->data(), nullptr);
}

TEST(JxxArrayParity, PositiveDimensionsRemainIndexable)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 2>>(2, 3);
    (*values)[1][2] = 42;
    EXPECT_EQ((*values)[1][2], 42);
    EXPECT_EQ(values->size(), 6U);
}
