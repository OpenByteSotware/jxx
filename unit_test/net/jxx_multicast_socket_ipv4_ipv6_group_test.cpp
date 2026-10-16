#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetAddress.h"
#include "net/jxx.net.MulticastSocket.h"
#include "net/jxx.net.SocketException.h"

TEST(MulticastSocketGroupParity, JoinsAndLeavesIpv4GroupByInetAddress) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    const auto group = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("239.255.0.1"));

    ASSERT_NE(nullptr, group);
    ASSERT_TRUE(group->isMulticastAddress());
    ASSERT_EQ(4, group->getAddress()->length);

    EXPECT_NO_THROW(socket->joinGroup(group));
    EXPECT_NO_THROW(socket->leaveGroup(group));
    socket->close();
}

TEST(MulticastSocketGroupParity, RejectsIpv4UnicastGroup) {
    auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(0);
    const auto unicast = ::jxx::net::InetAddress::getByName(
        ::jxx::NEW<::jxx::lang::String>("127.0.0.1"));

    ASSERT_NE(nullptr, unicast);
    ASSERT_FALSE(unicast->isMulticastAddress());
    EXPECT_THROW(socket->joinGroup(unicast),
        ::jxx::lang::IllegalArgumentException);
    socket->close();
}

TEST(MulticastSocketGroupParity, JoinsAndLeavesIpv6GroupByNativePath) {
    try {
        auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(
            ::jxx::net::Family::IPv6);
        const auto group = ::jxx::net::InetAddress::getByName(
            ::jxx::NEW<::jxx::lang::String>("ff02::1"));

        ASSERT_NE(nullptr, group);
        ASSERT_TRUE(group->isMulticastAddress());
        ASSERT_EQ(16, group->getAddress()->length);

        EXPECT_NO_THROW(socket->joinGroupV6("ff02::1", 0));
        EXPECT_NO_THROW(socket->leaveGroupV6("ff02::1", 0));
        socket->close();
    } catch (const ::jxx::net::SocketException&) {
        GTEST_SKIP() << "IPv6 multicast is unavailable on this host";
    }
}

TEST(MulticastSocketGroupParity, RejectsIpv6UnicastGroup) {
    try {
        auto socket = ::jxx::NEW<::jxx::net::MulticastSocket>(
            ::jxx::net::Family::IPv6);
        EXPECT_THROW(socket->joinGroupV6("::1", 0),
            ::jxx::lang::IllegalArgumentException);
        socket->close();
    } catch (const ::jxx::net::SocketException&) {
        GTEST_SKIP() << "IPv6 is unavailable on this host";
    }
}
