#include <gtest/gtest.h>

#include "net/jxx.net.DatagramSocket.h"

TEST(DatagramOptionStateTest, BroadcastAndReuseAddressRoundTrip) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_FALSE(socket.getBroadcast());
    EXPECT_FALSE(socket.getReuseAddress());
    socket.setBroadcast(true);
    socket.setReuseAddress(true);
    EXPECT_TRUE(socket.getBroadcast());
    EXPECT_TRUE(socket.getReuseAddress());
}

TEST(DatagramEndpointStateTest, ConnectAndDisconnectUpdateRemoteEndpoint) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    socket.connect("127.0.0.1", 9);
    EXPECT_TRUE(socket.isConnected());
    EXPECT_FALSE(socket.getRemoteAddress().empty());
    EXPECT_EQ(9, socket.getRemotePort());
    socket.disconnect();
    EXPECT_FALSE(socket.isConnected());
    EXPECT_TRUE(socket.getRemoteAddress().empty());
    EXPECT_EQ(0, socket.getRemotePort());
    socket.disconnect();
    EXPECT_FALSE(socket.isConnected());
}

TEST(DatagramLifecycleStateTest, CloseResetsCachedOptions) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    socket.setBroadcast(true);
    socket.setReuseAddress(true);
    socket.setSoTimeout(25);
    socket.close();
    EXPECT_FALSE(socket.getBroadcast());
    EXPECT_FALSE(socket.getReuseAddress());
    EXPECT_EQ(0, socket.getSoTimeout());
}
