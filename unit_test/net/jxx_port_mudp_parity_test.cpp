#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.MulticastSocket.h"

TEST(PortMudpParity, IntConstructorCreatesBoundIpv4Socket) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    EXPECT_TRUE(socket->isBound());
    EXPECT_GT(socket->getLocalPort(), 0);
    socket->close();
}

TEST(PortMudpParity, JoinAndLeaveGroupAcceptInetAddress) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    const auto group = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("239.255.0.1"));
    ASSERT_TRUE(group->isMulticastAddress());
    socket->joinGroup(group);
    socket->leaveGroup(group);
    socket->close();
}

TEST(PortMudpParity, RejectsNonMulticastJoinAddress) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    const auto address = ::jxx::net::InetAddress::getLoopbackAddress();
    EXPECT_THROW(socket->joinGroup(address),
        ::jxx::lang::IllegalArgumentException);
    socket->close();
}

TEST(PortMudpParity, TimeToLiveValidatesRange) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    socket->setTimeToLive(7);
    EXPECT_EQ(7, socket->getTimeToLive());
    EXPECT_THROW(socket->setTimeToLive(-1),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->setTimeToLive(256),
        ::jxx::lang::IllegalArgumentException);
    socket->close();
}
