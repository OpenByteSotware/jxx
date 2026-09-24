#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.MulticastSocket.h"

TEST(MulticastIpv4ValidationTest, RejectsNonMulticastAndInvalidInterfaceAddresses) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv4);
    EXPECT_THROW(socket.joinGroupIPv4("127.0.0.1"),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.leaveGroupIPv4("192.0.2.1"),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.joinGroupIPv4("239.1.2.3", "not-an-address"),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.setMulticastInterfaceIPv4("not-an-address"),
        ::jxx::lang::IllegalArgumentException);
}

TEST(MulticastIpv6ValidationTest, RejectsNonMulticastGroupAddresses) {
    ::jxx::net::DatagramSocket socket(::jxx::net::Family::IPv6);
    EXPECT_THROW(socket.joinGroupIPv6("::1"),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket.leaveGroupIPv6("2001:db8::1"),
        ::jxx::lang::IllegalArgumentException);
}

TEST(MulticastRangeValidationTest, TtlAndHopLimitUseUnsignedByteRange) {
    ::jxx::net::DatagramSocket ipv4(::jxx::net::Family::IPv4);
    EXPECT_THROW(ipv4.setMulticastTTL(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(ipv4.setMulticastTTL(256), ::jxx::lang::IllegalArgumentException);

    ::jxx::net::DatagramSocket ipv6(::jxx::net::Family::IPv6);
    EXPECT_THROW(ipv6.setMulticastHopsIPv6(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(ipv6.setMulticastHopsIPv6(256), ::jxx::lang::IllegalArgumentException);

    ::jxx::net::MulticastSocket multicast(::jxx::net::Family::IPv4);
    EXPECT_THROW(multicast.setTimeToLive(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(multicast.setTimeToLive(256), ::jxx::lang::IllegalArgumentException);
    EXPECT_EQ(1, multicast.getTimeToLive());
}
