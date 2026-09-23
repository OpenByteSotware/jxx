#include <gtest/gtest.h>

#include "lang/jxx.lang.NullPointerException.h"
#include "net/jxx.net.InetAddress.h"

TEST(InetAddressValueTest, RejectsNullAddressBytes) {
    EXPECT_THROW(
        ::jxx::net::InetAddress::getByAddress(nullptr),
        ::jxx::lang::NullPointerException);
}

TEST(InetAddressValueTest, ConstructionAndAccessUseDefensiveCopies) {
    auto bytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    (*bytes)[0] = 127;
    (*bytes)[1] = 0;
    (*bytes)[2] = 0;
    (*bytes)[3] = 1;

    const auto address = ::jxx::net::InetAddress::getByAddress(bytes);
    (*bytes)[0] = 10;
    EXPECT_EQ("127.0.0.1", address->getHostAddress()->utf8());

    auto returned = address->getAddress();
    (*returned)[0] = 10;
    EXPECT_EQ(127, static_cast<unsigned char>((*address->getAddress())[0]));
}

TEST(InetAddressValueTest, EqualityAndHashCodeUseAddressValue) {
    auto leftBytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    auto rightBytes = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    for (::jxx::lang::jint i = 0; i < 4; ++i) {
        (*leftBytes)[i] = static_cast<::jxx::lang::jbyte>(i + 1);
        (*rightBytes)[i] = static_cast<::jxx::lang::jbyte>(i + 1);
    }

    const auto left = ::jxx::net::InetAddress::getByAddress(leftBytes);
    const auto right = ::jxx::net::InetAddress::getByAddress(rightBytes);
    EXPECT_TRUE(left->equals(right));
    EXPECT_EQ(left->hashCode(), right->hashCode());
}
