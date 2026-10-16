#include <gtest/gtest.h>

#include "lang/jxx.lang.String.h"
#include "net/jxx.net.DatagramPacket.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.InetSocketAddress.h"
#include "net/jxx.net.SocketTimeoutException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace {
::jxx::Ptr<::jxx::net::InetSocketAddress> loopbackEphemeral() {
    return ::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"), 0);
}
}

TEST(PortUdpParity, HostnameSocketAddressBindsAndExposesLocalPort) {
    auto socket = ::jxx::NEW<::jxx::net::DatagramSocket>(
        loopbackEphemeral());
    EXPECT_TRUE(socket->isBound());
    EXPECT_GT(socket->getLocalPort(), 0);
    socket->close();
    EXPECT_TRUE(socket->isClosed());
}

TEST(PortUdpParity, DisconnectPreservesLocalBinding) {
    auto receiver = ::jxx::NEW<::jxx::net::DatagramSocket>(
        loopbackEphemeral());
    const auto localPort = receiver->getLocalPort();
    receiver->connect(
        ::jxx::net::InetAddress::getLoopbackAddress(), localPort);
    ASSERT_TRUE(receiver->isConnected());

    receiver->disconnect();

    EXPECT_FALSE(receiver->isConnected());
    EXPECT_TRUE(receiver->isBound());
    EXPECT_EQ(localPort, receiver->getLocalPort());
    receiver->close();
}

TEST(PortUdpParity, ReceiveHonorsPacketLengthNotBackingArrayCapacity) {
    auto receiver = ::jxx::NEW<::jxx::net::DatagramSocket>(
        loopbackEphemeral());
    auto sender = ::jxx::NEW<::jxx::net::DatagramSocket>(
        loopbackEphemeral());

    auto payload = ::jxx::NEW<::jxx::lang::ByteArrayType>(8);
    for (::jxx::lang::jint index = 0; index < 8; ++index) {
        (*payload)[index] = static_cast<::jxx::lang::jbyte>(index + 1);
    }
    auto destination = ::jxx::NEW<::jxx::net::InetSocketAddress>(
        ::jxx::net::InetAddress::getLoopbackAddress(),
        receiver->getLocalPort());
    auto outgoing = ::jxx::NEW<::jxx::net::DatagramPacket>(
        payload, 8, destination);
    sender->send(outgoing);

    auto backing = ::jxx::NEW<::jxx::lang::ByteArrayType>(16);
    auto incoming = ::jxx::NEW<::jxx::net::DatagramPacket>(
        backing, 2, 3);
    receiver->setSoTimeout(2000);
    receiver->receive(incoming);

    EXPECT_EQ(3, incoming->getLength());
    EXPECT_EQ(1, static_cast<int>((*backing)[2]));
    EXPECT_EQ(3, static_cast<int>((*backing)[4]));

    sender->close();
    receiver->close();
}

TEST(PortUdpParity, NegativeTimeoutIsRejected) {
    auto socket = ::jxx::NEW<::jxx::net::DatagramSocket>(
        loopbackEphemeral());
    EXPECT_THROW(socket->setSoTimeout(-1), ::jxx::lang::IllegalArgumentException);
    socket->close();
}
