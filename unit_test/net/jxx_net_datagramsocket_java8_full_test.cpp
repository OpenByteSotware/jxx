#include <gtest/gtest.h>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.SocketException.h"

TEST(DatagramPacketParityTest, OffsetLengthAndDataMutatorsValidateRanges) {
    ::jxx::net::DatagramPacket packet;
    packet.setData(std::vector<std::uint8_t>{1,2,3,4},1,2);
    EXPECT_EQ(1U,packet.offset); EXPECT_EQ(2U,packet.length);
    packet.setLength(3); EXPECT_EQ(3U,packet.length);
    EXPECT_THROW(packet.setLength(4),::jxx::lang::IllegalArgumentException);
}
TEST(DatagramSocketFullParityTest, OptionsEndpointsAndLifecycleAreCoherent) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    socket.setSoTimeout(25); EXPECT_EQ(25,socket.getSoTimeout());
    socket.setBroadcast(true); EXPECT_TRUE(socket.getBroadcast());
    socket.setReuseAddress(true); EXPECT_TRUE(socket.getReuseAddress());
    socket.setSendBufferSize(4096); EXPECT_GT(socket.getSendBufferSize(),0);
    socket.setReceiveBufferSize(4096); EXPECT_GT(socket.getReceiveBufferSize(),0);
    socket.bind(0); EXPECT_TRUE(socket.isBound()); EXPECT_NE(0,socket.getLocalPort());
    socket.close(); EXPECT_TRUE(socket.isClosed());
}
TEST(DatagramSocketFamilyAwareBindTest, ExplicitIpv6AnyAddressBinds) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv6);
    socket.bind("::",0); EXPECT_TRUE(socket.isBound()); EXPECT_NE(0,socket.getLocalPort());
}
