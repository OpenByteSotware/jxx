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
    EXPECT_EQ(values->length, 0);
    EXPECT_EQ(values->size(), 0U);
    EXPECT_EQ(values->data(), nullptr);
}

TEST(JxxArrayParity, HigherRankAllowsZeroDimension)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jint, 2>>(3, 0);

    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->length, 3);
    EXPECT_EQ(values->size(), 3U);

    for (::jxx::lang::jint rowIndex = 0;
         rowIndex < values->length;
         ++rowIndex) {
        const auto& row = values->row(rowIndex);
        ASSERT_NE(row, nullptr);
        EXPECT_EQ(row->length, 0);
        EXPECT_EQ(row->size(), 0U);
        EXPECT_EQ(row->data(), nullptr);
    }
}

TEST(JxxArrayParity, PositiveDimensionsRemainIndexable)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<
        ::jxx::lang::JxxArray<::jxx::lang::jint, 2>>(2, 3);

    (*values)[1][2] = 42;

    EXPECT_EQ((*values)[1][2], 42);
    EXPECT_EQ(values->length, 2);
    EXPECT_EQ(values->size(), 2U);

    for (::jxx::lang::jint rowIndex = 0;
         rowIndex < values->length;
         ++rowIndex) {
        const auto& row = values->row(rowIndex);
        ASSERT_NE(row, nullptr);
        EXPECT_EQ(row->length, 3);
        EXPECT_EQ(row->size(), 3U);
    }
}
