#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.DatagramPacket.h"
#include "net/jxx.net.InetAddress.h"

TEST(PortAbsEthernetParity, DatagramPacketRetainsClientBufferAndRange) {
    auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(32);
    auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(data, 32);

    EXPECT_EQ(data.get(), packet->getData().get());
    EXPECT_EQ(0, packet->getOffset());
    EXPECT_EQ(32, packet->getLength());

    packet->setLength(7);
    EXPECT_EQ(7, packet->getLength());
}

TEST(PortAbsEthernetParity, InvalidSetDataDoesNotPartiallyMutatePacket) {
    auto original = ::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    auto replacement = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(original, 8);

    EXPECT_THROW(packet->setData(replacement, 3, 2),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_EQ(original.get(), packet->getData().get());
    EXPECT_EQ(0, packet->getOffset());
    EXPECT_EQ(8, packet->getLength());
}

TEST(PortAbsEthernetParity, NumericAddressToStringSupportsSubstringOne) {
    auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));
    ASSERT_NE(nullptr, address);
    const auto text = address->toString();
    ASSERT_NE(nullptr, text);
    EXPECT_EQ("/127.0.0.1", text->utf8());
    EXPECT_EQ("127.0.0.1", text->substring(1)->utf8());
}

TEST(PortAbsEthernetParity, NullHostResolvesToLoopback) {
    const auto address = ::jxx::net::InetAddress::getByName(nullptr);
    ASSERT_NE(nullptr, address);
    EXPECT_TRUE(address->isLoopbackAddress());
}
