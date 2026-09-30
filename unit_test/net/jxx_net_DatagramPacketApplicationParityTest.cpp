#include <gtest/gtest.h>

#include "lang/jxx.lang.buildin_array.h"
#include "net/jxx.net.DatagramPacket.h"
#include "net/jxx.net.InetAddress.h"

namespace {

TEST(DatagramPacketApplicationParity, ReceiveBufferIdentityIsPreserved) {
    const auto buffer = ::jxx::NEW<::jxx::lang::ByteArrayType>(64);
    const auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(buffer, 64);

    EXPECT_EQ(buffer, packet->getData());
    EXPECT_EQ(0, packet->getOffset());
    EXPECT_EQ(64, packet->getLength());
}

TEST(DatagramPacketApplicationParity, OutgoingPacketCarriesDataAddressAndPort) {
    const auto data = ::jxx::NEW<::jxx::lang::ByteArrayType>(4);
    const auto address = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));
    const auto packet = ::jxx::NEW<::jxx::net::DatagramPacket>(
        data,
        data->length,
        address,
        12345);

    EXPECT_EQ(data, packet->getData());
    EXPECT_EQ(address, packet->getAddress());
    EXPECT_EQ(12345, packet->getPort());
    EXPECT_EQ(4, packet->getLength());
}

} // namespace
