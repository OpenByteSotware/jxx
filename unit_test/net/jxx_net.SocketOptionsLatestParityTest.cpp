#include <gtest/gtest.h>
#include "net/jxx.net.SocketOptions.h"
namespace {
TEST(SocketOptionsLatestParityTest, UsesUnderscoreConstants) {
    EXPECT_EQ(::jxx::net::SocketOptions::TCP_NODELAY_, 0x0001);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_TIMEOUT_, 0x1006);
}
}
