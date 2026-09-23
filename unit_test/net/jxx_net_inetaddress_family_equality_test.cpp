#include <gtest/gtest.h>
#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.InetAddress.h"
namespace {
TEST(InetAddressFamilyEqualityTest, DifferentAddressLengthsAreNotEqual){auto a=::jxx::NEW<::jxx::lang::ByteArrayType>(4);auto b=::jxx::NEW<::jxx::lang::ByteArrayType>(16);EXPECT_FALSE(::jxx::net::InetAddress::getByAddress(a)->equals(::jxx::net::InetAddress::getByAddress(b)));}
}
