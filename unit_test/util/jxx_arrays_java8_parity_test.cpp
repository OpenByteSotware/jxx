#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Arrays.h"

TEST(ArraysJava8ParityTest, ParallelSortAndRangeBinarySearch)
{
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(5);
    (*values)[0] = 5; (*values)[1] = 4; (*values)[2] = 3;
    (*values)[3] = 2; (*values)[4] = 1;
    ::jxx::util::Arrays::parallelSort(values, 1, 4);
    EXPECT_EQ(5, (*values)[0]);
    EXPECT_EQ(2, (*values)[1]);
    EXPECT_EQ(3, (*values)[2]);
    EXPECT_EQ(4, (*values)[3]);
    EXPECT_EQ(1, (*values)[4]);
    EXPECT_EQ(2, ::jxx::util::Arrays::binarySearch(values, 1, 4, 3));
}

TEST(ArraysJava8ParityTest, SetAllParallelSetAllAndPrefix)
{
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(4);
    ::jxx::util::Arrays::setAll(values, [](::jxx::lang::jint i) {
        return i * 2;
    });
    ::jxx::util::Arrays::parallelSetAll(values, [](::jxx::lang::jint i) {
        return i + 1;
    });
    ::jxx::util::Arrays::parallelPrefix(values,
        [](::jxx::lang::jint a, ::jxx::lang::jint b) { return a + b; });
    EXPECT_EQ(1, (*values)[0]);
    EXPECT_EQ(3, (*values)[1]);
    EXPECT_EQ(6, (*values)[2]);
    EXPECT_EQ(10, (*values)[3]);
}

TEST(ArraysJava8ParityTest, StreamsAndReferenceSpliterator)
{
    auto values = ::jxx::NEW<::jxx::lang::IntArrayType>(3);
    (*values)[0] = 1; (*values)[1] = 2; (*values)[2] = 3;
    EXPECT_EQ(6, ::jxx::util::Arrays::stream(values)->sum());
    EXPECT_EQ(5, ::jxx::util::Arrays::stream(values, 1, 3)->sum());

    auto strings = ::jxx::NEW<::jxx::lang::JxxArray<
        ::jxx::Ptr<::jxx::lang::String>, 1U>>(1);
    (*strings)[0] = ::jxx::NEW<::jxx::lang::String>("x");
    EXPECT_NE(nullptr, ::jxx::util::Arrays::spliterator(strings));
}
