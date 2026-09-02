#include <gtest/gtest.h>
#include <type_traits>
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {
TEST(JxxArrayTest, PrimitiveRankOneHasPublicLength) {
    auto values = jxx::NEW<jxx::lang::IntArrayType>(3);
    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->length, 3U);
    (*values)[0] = 10; (*values)[1] = 20; (*values)[2] = 30;
    EXPECT_EQ((*values)[1], 20);
    EXPECT_THROW(values->at(3), std::out_of_range);
}

TEST(JxxArrayTest, ObjectReferenceArrayStoresJavaReferences) {
    using String = jxx::lang::String;
    auto values = jxx::NEW<jxx::lang::JxxArray<jxx::Ptr<String>, 1U>>(2);
    auto shared = jxx::NEW<String>("shared");
    (*values)[0] = shared;
    (*values)[1] = nullptr;
    EXPECT_EQ(values->length, 2U);
    EXPECT_EQ((*values)[0].get(), shared.get());
    EXPECT_EQ((*values)[1], nullptr);
}

TEST(JxxArrayTest, RankTwoConstructionExposesOuterLength) {
    auto values = jxx::NEW<jxx::lang::IntArray2DType>(2, 3);
    ASSERT_NE(values, nullptr);
    EXPECT_EQ(values->length, 2U);
}
} // namespace
