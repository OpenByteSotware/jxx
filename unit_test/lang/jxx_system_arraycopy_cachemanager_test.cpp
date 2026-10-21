#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "lang/jxx.lang.System.h"
#include "lang/jxx.lang.JxxArray.h"

TEST(SystemArraycopyParity, CopiesPacketPayloadAndSupportsOverlap)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::lang::ByteArrayType>(5);
    for (::jxx::lang::jint index = 0; index < 5; ++index)
        (*source)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    auto destination = ::jxx::NEW<::jxx::lang::ByteArrayType>(9);
    ::jxx::lang::System::arraycopy(source, 0, destination, 2, 5);
    EXPECT_EQ((*destination)[2], 1);
    EXPECT_EQ((*destination)[6], 5);

    ::jxx::lang::System::arraycopy(destination, 2, destination, 3, 5);
    EXPECT_EQ((*destination)[3], 1);
    EXPECT_EQ((*destination)[7], 5);
}

TEST(SystemArraycopyParity, ZeroLengthAtArrayEndIsValid)
{
    ::jxx::initialize();
    auto array = ::jxx::NEW<::jxx::lang::ByteArrayType>(1);
    EXPECT_NO_THROW(::jxx::lang::System::arraycopy(array, 1, array, 1, 0));
}
