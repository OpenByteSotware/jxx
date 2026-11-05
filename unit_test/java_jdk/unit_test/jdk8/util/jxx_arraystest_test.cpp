#include <gtest/gtest.h>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.GenericConversion.h"
#include "lang/jxx.lang.JxxArray.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "util/jxx.util.Arrays.h"
#include "util/jxx.util.Collections.h"
#include "util/jxx.util.List.h"
#include "util/jxx.util.Objects.h"

namespace jxx::jdk8::util {
    TEST(ArraysTest, SortAndParallelSortOrderValues) {
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> a = ::jxx::NEW<::jxx::lang::IntArrayType>(3U);
        (*a)[0U] = 3;
        (*a)[1U] = 1;
        (*a)[2U] = 2;
        ::jxx::util::Arrays::sort(a);
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> b = ::jxx::NEW<::jxx::lang::IntArrayType>(5U);
        (*b)[0U] = 5;
        (*b)[1U] = 4;
        (*b)[2U] = 3;
        (*b)[3U] = 2;
        (*b)[4U] = 1;
        ::jxx::util::Arrays::parallelSort(b, 1, 4);
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), b);
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 2U>> s = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>, 1U>>(3U);
        (*s)[0U] = "b";
        (*s)[1U] = "a";
        (*s)[2U] = "c";
        ::jxx::util::Arrays::sort(s, ::jxx::util::Collections::reverseOrder());
        assertArrayEquals(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>(), s);
    }

    TEST(ArraysTest, BinarySearchFindsValuesAndInsertionPoints) {
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> a = ::jxx::NEW<::jxx::lang::IntArrayType>(4U);
        (*a)[0U] = 1;
        (*a)[1U] = 3;
        (*a)[2U] = 5;
        (*a)[3U] = 7;
        EXPECT_EQ(2, ::jxx::util::Arrays::binarySearch(a, 5));
        EXPECT_EQ(-3, ::jxx::util::Arrays::binarySearch(a, 4));
        EXPECT_EQ(1, ::jxx::util::Arrays::binarySearch(a, 1, 3, 3));
    }

    TEST(ArraysTest, EqualityDeepEqualityAndHashingWork) {
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> a = ::jxx::NEW<::jxx::lang::IntArrayType>(2U);
        (*a)[0U] = 1;
        (*a)[1U] = 2;
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 2U>> b = ::jxx::NEW<::jxx::lang::IntArrayType>(2U);
        (*b)[0U] = 1;
        (*b)[1U] = 2;
        EXPECT_TRUE(::jxx::util::Arrays::equals(a, b));
        EXPECT_EQ(::jxx::util::Arrays::hashCode(a), ::jxx::util::Arrays::hashCode(b));
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 2U>> x = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>, 1U>>(2U);
        (*x)[0U] = ::jxx::NEW<::jxx::lang::IntArrayType>();
        (*x)[1U] = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>();
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 2U>> y = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>, 1U>>(2U);
        (*y)[0U] = ::jxx::NEW<::jxx::lang::IntArrayType>();
        (*y)[1U] = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>();
        EXPECT_TRUE(::jxx::util::Arrays::deepEquals(x, y));
        EXPECT_EQ(::jxx::util::Arrays::deepHashCode(x), ::jxx::util::Arrays::deepHashCode(y));
    }

    TEST(ArraysTest, FillCopyAndRangeCopyWork) {
        ::jxx::lang::IntArray a = ::jxx::NEW<::jxx::lang::IntArrayType>(4);
        ::jxx::util::Arrays::fill(a, 7);
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
        ::jxx::util::Arrays::fill(a, 1, 3, 9);
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), ::jxx::util::Arrays::copyOf(a, 6));
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), ::jxx::util::Arrays::copyOfRange(a, 1, 3));
    }

    TEST(ArraysTest, ListAndStringRepresentationsWork) {
        ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 2U>> a = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>, 1U>>(2U);
        (*a)[0U] = "a";
        (*a)[1U] = "b";
        ::jxx::Ptr<::jxx::util::List<::jxx::lang::String>> list = ::jxx::util::Arrays::asList(a);
        list->set(0, ::jxx::NEW<::jxx::lang::String>("x"));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("x"), (*a)[0]));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("[1, 2]"), ::jxx::lang::String::valueOf(::jxx::util::Arrays::toString(::jxx::NEW<::jxx::lang::IntArrayType>()))));
        EXPECT_TRUE(::jxx::util::Objects::equals(::jxx::NEW<::jxx::lang::String>("[[1, 2]]"), ::jxx::util::Arrays::deepToString(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::Object>, 1U>>())));
    }

    TEST(ArraysTest, SetAllParallelSetAllAndPrefixWork) {
        ::jxx::lang::IntArray a = ::jxx::NEW<::jxx::lang::IntArrayType>(4);
        ::jxx::util::Arrays::setAll(a, [&](auto i) {
    (i * 2);
});
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
        ::jxx::util::Arrays::parallelSetAll(a, [&](auto i) {
    (i + 1);
});
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
        ::jxx::util::Arrays::parallelPrefix(a, [&](auto x, auto y) {
    (x + y);
});
        assertArrayEquals(::jxx::NEW<::jxx::lang::IntArrayType>(), a);
    }

    TEST(ArraysTest, StreamsCoverFullAndPartialRanges) {
        EXPECT_EQ(6, ::jxx::util::Arrays::stream(::jxx::NEW<::jxx::lang::IntArrayType>())->sum());
        EXPECT_EQ(5, ::jxx::util::Arrays::stream(::jxx::NEW<::jxx::lang::IntArrayType>(), 1, 3)->sum());
        EXPECT_NE(::jxx::util::Arrays::spliterator(::jxx::NEW<::jxx::lang::JxxArray<::jxx::Ptr<::jxx::lang::String>, 1U>>()), nullptr);
    }

} // namespace jxx::jdk8::util
