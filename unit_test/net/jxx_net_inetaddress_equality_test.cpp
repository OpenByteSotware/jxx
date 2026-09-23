#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"

namespace {
TEST(InetAddressEqualityTest, EqualAddressBytesCompareEqualAndShareHash) {
    auto leftBytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    auto rightBytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    for (::jxx::lang::jint index = 0; index < 4; ++index) {
        (*leftBytes)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
        (*rightBytes)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    }
    const auto left = ::jxx::net::InetAddress::getByAddress(leftBytes);
    const auto right = ::jxx::net::InetAddress::getByAddress(rightBytes);
    EXPECT_TRUE(left->equals(right));
    EXPECT_TRUE(right->equals(left));
    EXPECT_EQ(left->hashCode(), right->hashCode());
}
}
