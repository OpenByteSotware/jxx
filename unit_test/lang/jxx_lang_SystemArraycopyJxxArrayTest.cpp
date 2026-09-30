#include <gtest/gtest.h>

#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

TEST(SystemArraycopyJxxArrayParity, CopiesByteArrayRange) {
    const auto source = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    const auto destination = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    for (::jxx::lang::jint index = 0; index < 5; ++index) {
        (*source)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    }

    ::jxx::lang::System::arraycopy(source, 1, destination, 2, 3);

    EXPECT_EQ(2, (*destination)[2]);
    EXPECT_EQ(3, (*destination)[3]);
    EXPECT_EQ(4, (*destination)[4]);
}

TEST(SystemArraycopyJxxArrayParity, OverlappingCopyUsesArraySemantics) {
    const auto values = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    for (::jxx::lang::jint index = 0; index < 5; ++index) {
        (*values)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    }

    ::jxx::lang::System::arraycopy(values, 0, values, 1, 4);

    EXPECT_EQ(1, (*values)[0]);
    EXPECT_EQ(1, (*values)[1]);
    EXPECT_EQ(2, (*values)[2]);
    EXPECT_EQ(3, (*values)[3]);
    EXPECT_EQ(4, (*values)[4]);
}

TEST(SystemArraycopyJxxArrayParity, NullAndBoundsChecks) {
    const auto values = ::jxx::NEW<::jxx::lang::ByteArrayType>(3);
    const ::jxx::lang::ByteArray nullArray;

    EXPECT_THROW(
        ::jxx::lang::System::arraycopy(nullArray, 0, values, 0, 1),
        ::jxx::lang::NullPointerException);

    EXPECT_THROW(
        ::jxx::lang::System::arraycopy(values, 0, nullArray, 0, 1),
        ::jxx::lang::NullPointerException);

    EXPECT_THROW(
        ::jxx::lang::System::arraycopy(values, 0, values, 0, 4),
        ::jxx::lang::IndexOutOfBoundsException);
}

} // namespace
