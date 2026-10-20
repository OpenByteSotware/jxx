#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "util/jxx.util.Arrays.h"

TEST(ArraysWavFilesParity, FillInitializesWholePrimitiveArray)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(256);
    ::jxx::util::Arrays::fill<::jxx::lang::jint>(values, -128);
    ASSERT_EQ(values->size(), 256U);
    for (std::size_t index = 0; index < values->size(); ++index)
        EXPECT_EQ((*values)[index], -128);
}

TEST(ArraysWavFilesParity, CopyOfRangeProducesRequestedFloorTable)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(5);
    for (::jxx::lang::jint index = 0; index < 5; ++index)
        (*values)[index] = index + 10;

    auto result = ::jxx::util::Arrays::copyOfRange<::jxx::lang::jint>(
        values, 1, 4);
    ASSERT_EQ(result->size(), 3U);
    EXPECT_EQ((*result)[0], 11);
    EXPECT_EQ((*result)[1], 12);
    EXPECT_EQ((*result)[2], 13);
}

TEST(ArraysWavFilesParity, CopyOfRangePadsBeyondSourceLength)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(2);
    (*values)[0] = 7;
    (*values)[1] = 8;
    auto result = ::jxx::util::Arrays::copyOfRange<::jxx::lang::jint>(
        values, 1, 4);
    ASSERT_EQ(result->size(), 3U);
    EXPECT_EQ((*result)[0], 8);
    EXPECT_EQ((*result)[1], 0);
    EXPECT_EQ((*result)[2], 0);
}

TEST(ArraysWavFilesParity, CopyOfRangeValidatesRange)
{
    ::jxx::initialize();
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(2);
    EXPECT_THROW(
        (::jxx::util::Arrays::copyOfRange<::jxx::lang::jint>(values, 2, 1)),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(
        (::jxx::util::Arrays::copyOfRange<::jxx::lang::jint>(values, -1, 1)),
        ::jxx::lang::IndexOutOfBoundsException);
}
