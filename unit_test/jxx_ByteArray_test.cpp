#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"

namespace {
TEST(ByteArrayTest, AllocationLengthAndIndexingWork) {
    auto bytes = jxx::NEW<jxx::lang::ByteArrayType>(4);
    ASSERT_NE(bytes, nullptr);
    EXPECT_EQ(bytes->length, 4U);
    (*bytes)[0] = static_cast<jxx::lang::jbyte>(0x12);
    (*bytes)[1] = static_cast<jxx::lang::jbyte>(0x34);
    EXPECT_EQ(static_cast<unsigned int>((*bytes)[0]) & 0xFFU, 0x12U);
    EXPECT_EQ(static_cast<unsigned int>((*bytes)[1]) & 0xFFU, 0x34U);
}

TEST(ByteArrayTest, FillAndBoundsCheckWork) {
    auto bytes = jxx::NEW<jxx::lang::ByteArrayType>(3);
    bytes->fill(static_cast<jxx::lang::jbyte>(7));
    EXPECT_EQ((*bytes)[2], static_cast<jxx::lang::jbyte>(7));
    EXPECT_THROW(bytes->at(3), std::out_of_range);
}
} // namespace
