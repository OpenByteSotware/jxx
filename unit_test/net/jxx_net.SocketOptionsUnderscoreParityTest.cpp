#include <gtest/gtest.h>

#include "net/jxx.net.SocketOptions.h"

namespace {

TEST(SocketOptionsUnderscoreParityTest, ConstantsAvoidPlatformMacros) {
    EXPECT_EQ(::jxx::net::SocketOptions::TCP_NODELAY_, 0x0001);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_REUSEADDR_, 0x0004);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_KEEPALIVE_, 0x0008);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_BINDADDR_, 0x000F);
    EXPECT_EQ(::jxx::net::SocketOptions::IP_MULTICAST_IF_, 0x0010);
    EXPECT_EQ(::jxx::net::SocketOptions::IP_MULTICAST_LOOP_, 0x0012);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_BROADCAST_, 0x0020);
    EXPECT_EQ(::jxx::net::SocketOptions::IP_MULTICAST_IF2_, 0x001F);
    EXPECT_EQ(::jxx::net::SocketOptions::IP_TOS_, 0x0003);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_LINGER_, 0x0080);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_SNDBUF_, 0x1001);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_RCVBUF_, 0x1002);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_OOBINLINE_, 0x1003);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_TIMEOUT_, 0x1006);
}

} // namespace
