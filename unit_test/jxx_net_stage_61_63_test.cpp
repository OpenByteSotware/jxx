#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.DatagramSocket.h"

TEST(DatagramBufferOptionTest, RejectsNonPositiveSizes) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_THROW(socket.setSendBufferSize(0), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.setReceiveBufferSize(-1), ::jxx::lang::IllegalArgumentException);
}

TEST(DatagramBufferOptionTest, PositiveSizesRoundTripThroughNativeSocket) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    socket.setSendBufferSize(4096);
    socket.setReceiveBufferSize(4096);
    EXPECT_GT(socket.getSendBufferSize(), 0);
    EXPECT_GT(socket.getReceiveBufferSize(), 0);
}

TEST(DatagramImplicitBindTest, ConnectAssignsLocalEndpointAndBoundState) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_FALSE(socket.isBound());
    socket.connect("127.0.0.1", 9);
    EXPECT_TRUE(socket.isBound());
    EXPECT_NE(0, socket.getLocalPort());
    EXPECT_FALSE(socket.getLocalAddress().empty());
}

TEST(DatagramImplicitBindTest, SendAssignsLocalEndpointAndBoundState) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    ::jxx::net::DatagramPacket packet(
        std::vector<std::uint8_t>{1}, "127.0.0.1", 9);
    socket.send(packet);
    EXPECT_TRUE(socket.isBound());
    EXPECT_NE(0, socket.getLocalPort());
    EXPECT_FALSE(socket.getLocalAddress().empty());
}
