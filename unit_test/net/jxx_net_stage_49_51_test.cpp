#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.MulticastSocket.h"
#include "net/jxx.net.SocketException.h"

TEST(DatagramSocketValidationTest, PacketLengthCannotExceedBuffer) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    ::jxx::net::DatagramPacket packet(1);
    packet.length = 2;
    packet.address = "127.0.0.1";
    packet.port = 9;
    EXPECT_THROW(socket.send(packet), ::jxx::lang::IllegalArgumentException);
}

TEST(DatagramSocketLifecycleTest, TimeoutAndDuplicateBindAreRejected) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_THROW(socket.setSoTimeout(-1), ::jxx::lang::IllegalArgumentException);
    socket.bind("127.0.0.1", 0);
    EXPECT_TRUE(socket.isBound());
    EXPECT_THROW(socket.bind("127.0.0.1", 0), ::jxx::net::SocketException);
    socket.close();
    socket.close();
    EXPECT_TRUE(socket.isClosed());
    EXPECT_THROW(socket.setSoTimeout(0), ::jxx::net::SocketException);
}

TEST(MulticastSocketValidationTest, TimeToLiveMustBeWithinUnsignedByteRange) {
    ::jxx::net::MulticastSocket socket(::jxx::net::Family::IPv4);
    EXPECT_THROW(socket.setTimeToLive(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.setTimeToLive(256), ::jxx::lang::IllegalArgumentException);
    EXPECT_EQ(1, socket.getTimeToLive());
}
