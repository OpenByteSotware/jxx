#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"
#include "net/jxx.net.DatagramPacket.h"
#include "net/jxx.net.InetAddress.h"

TEST(PortAbsEthernetParity, EmptyHostResolvesToLoopback) {
    const auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>(""));
    ASSERT_NE(nullptr, address);
    EXPECT_TRUE(address->isLoopbackAddress());
}

TEST(PortAbsEthernetParity, ReceivedPacketAddressSupportsClientSubstring) {
    const auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));
    const auto text = address->toString();
    ASSERT_NE(nullptr, text);
    EXPECT_EQ("127.0.0.1", text->substring(1)->utf8());
}

TEST(PortAbsEthernetParity, PacketDataAndClientBufferShareIdentity) {
    auto buffer = ::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    (*buffer)[0] = 11;
    (*buffer)[1] = 22;
    auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(buffer, 2);
    EXPECT_EQ(buffer.get(), packet->getData().get());
    ::jxx::lang::System::arraycopy(
        packet->getData(), 0, buffer, 0, packet->getLength());
    EXPECT_EQ(11, static_cast<int>((*buffer)[0]));
    EXPECT_EQ(22, static_cast<int>((*buffer)[1]));
}

TEST(PortAbsEthernetParity, OutgoingPacketRetainsPayloadAddressAndPort) {
    auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    const auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));
    auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(
        data, data->length, address, 4321);
    EXPECT_EQ(data.get(), packet->getData().get());
    EXPECT_EQ(data->length, packet->getLength());
    EXPECT_TRUE(packet->getAddress()->equals(
        ::jxx::CAST<::jxx::lang::Object>(address)));
    EXPECT_EQ(4321, packet->getPort());
}
