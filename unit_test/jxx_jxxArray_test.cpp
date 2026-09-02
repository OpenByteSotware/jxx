#include <stdexcept>
#include <type_traits>

#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Exceptions.h"

namespace
{

    TEST(JxxArrayTest, PrimitiveRankOneHasPublicLength)
    {
        auto values =
            jxx::NEW<jxx::lang::IntArrayType>(
                static_cast<jxx::lang::jint>(3));

        ASSERT_NE(values, nullptr);
        EXPECT_EQ(values->length, 3);

        (*values)[0] = 10;
        (*values)[1] = 20;
        (*values)[2] = 30;

        EXPECT_EQ((*values)[1], 20);

        EXPECT_THROW(values->at(3), jxx::lang::ArrayIndexOutOfBoundsException);
    }

    TEST(JxxArrayTest, ObjectReferenceArrayStoresJavaReferences)
    {
        using String = jxx::lang::String;

        using StringArray =
            jxx::lang::JxxArray<
            jxx::Ptr<String>,
            1U>;

        static_assert(
            std::is_base_of_v<
                jxx::lang::Object,
                StringArray>,
            "Every Java array must inherit Object");

        static_assert(
            std::is_constructible_v<
                StringArray,
                jxx::lang::jint>,
            "Rank-one array must accept a Java int length");

        auto values =
            jxx::NEW<StringArray>(
                static_cast<jxx::lang::jint>(2));

        ASSERT_NE(values, nullptr);

        auto shared =
            jxx::NEW<String>("shared");

        (*values)[0] = shared;
        (*values)[1] = nullptr;

        EXPECT_EQ(values->length, 2);
        EXPECT_EQ((*values)[0].get(), shared.get());
        EXPECT_EQ((*values)[1], nullptr);
    }

    TEST(JxxArrayTest, RankTwoRectangularConstruction)
    {
        auto values =
            jxx::NEW<jxx::lang::IntArray2DType>(
                static_cast<jxx::lang::jint>(2),
                static_cast<jxx::lang::jint>(3));

        ASSERT_NE(values, nullptr);

        EXPECT_EQ(values->length, 2);

        ASSERT_NE(values->row(0), nullptr);
        ASSERT_NE(values->row(1), nullptr);

        EXPECT_EQ(values->row(0)->length, 3);
        EXPECT_EQ(values->row(1)->length, 3);

        (*values)[0][0] = 10;
        (*values)[0][1] = 20;
        (*values)[1][2] = 30;

        EXPECT_EQ((*values)[0][0], 10);
        EXPECT_EQ((*values)[0][1], 20);
        EXPECT_EQ((*values)[1][2], 30);
    }

    TEST(JxxArrayTest, RankThreeRectangularConstruction)
    {
        auto values =
            jxx::NEW<jxx::lang::IntArray3DType>(
                static_cast<jxx::lang::jint>(6),
                static_cast<jxx::lang::jint>(9),
                static_cast<jxx::lang::jint>(1));

        ASSERT_NE(values, nullptr);

        EXPECT_EQ(values->length, 6);
        EXPECT_EQ(values->row(0)->length, 9);
        EXPECT_EQ(values->row(0)->row(0)->length, 1);

        (*values)[5][8][0] = 42;

        EXPECT_EQ((*values)[5][8][0], 42);
    }

    TEST(JxxArrayTest, PartialRankTwoAllocationHasNullRows)
    {
        /*
         * Java:
         *
         * int[][] values = new int[2][];
         */
        auto values =
            jxx::NEW<jxx::lang::IntArray2DType>(
                static_cast<jxx::lang::jint>(2));

        ASSERT_NE(values, nullptr);
        EXPECT_EQ(values->length, 2);

        EXPECT_EQ(values->row(0), nullptr);
        EXPECT_EQ(values->row(1), nullptr);

        values->row(0) =
            jxx::NEW<jxx::lang::IntArrayType>(
                static_cast<jxx::lang::jint>(5));

        ASSERT_NE(values->row(0), nullptr);
        EXPECT_EQ(values->row(0)->length, 5);

        (*values)[0][4] = 99;
        EXPECT_EQ((*values)[0][4], 99);
    }

    TEST(JxxArrayTest, MultidimensionalArraysCanBeJagged)
    {
        /*
         * Java:
         *
         * int[][] values = new int[3][];
         * values[0] = new int[2];
         * values[1] = new int[5];
         * values[2] = null;
         */
        auto values =
            jxx::NEW<jxx::lang::IntArray2DType>(
                static_cast<jxx::lang::jint>(3));

        values->row(0) =
            jxx::NEW<jxx::lang::IntArrayType>(
                static_cast<jxx::lang::jint>(2));

        values->row(1) =
            jxx::NEW<jxx::lang::IntArrayType>(
                static_cast<jxx::lang::jint>(5));

        ASSERT_NE(values->row(0), nullptr);
        ASSERT_NE(values->row(1), nullptr);

        EXPECT_EQ(values->length, 3);
        EXPECT_EQ(values->row(0)->length, 2);
        EXPECT_EQ(values->row(1)->length, 5);
        EXPECT_EQ(values->row(2), nullptr);
    }

} // namespace