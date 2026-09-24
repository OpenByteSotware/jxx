#include <gtest/gtest.h>

#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.SocketException.h"
#include "net/jxx.net.UnknownHostException.h"

TEST(DatagramResolutionParityTest, UnknownDestinationUsesUnknownHostException) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    ::jxx::net::DatagramPacket packet(
        std::vector<std::uint8_t>{1}, "invalid.invalid", 9);
    EXPECT_THROW(socket.send(packet), ::jxx::net::UnknownHostException);
}

TEST(DatagramTimeoutStateTest, TimeoutRoundTripsAndCloseResetsState) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_EQ(0, socket.getSoTimeout());
    socket.setSoTimeout(25);
    EXPECT_EQ(25, socket.getSoTimeout());
    socket.close();
    EXPECT_EQ(0, socket.getSoTimeout());
}

TEST(DatagramFamilyParityTest, InvalidLoopbackFamilyUsesSocketException) {
    ::jxx::net::DatagramSocket ipv4(::jxx::net::Family::IPv4);
    EXPECT_THROW(ipv4.setMulticastLoopIPv6(true), ::jxx::net::SocketException);

    ::jxx::net::DatagramSocket ipv6(::jxx::net::Family::IPv6);
    EXPECT_THROW(ipv6.setMulticastLoopIPv4(true), ::jxx::net::SocketException);
}
