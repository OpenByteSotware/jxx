#include <gtest/gtest.h>
#include <type_traits>

#include "net/jxx.net.SocketOption.h"
#include "net/jxx.net.SocketOptions.h"
#include "net/jxx.net.StandardSocketOptions.h"

namespace {

TEST(SocketOptionParityTest, InterfacesDoNotInheritObject) {
    EXPECT_FALSE((std::is_base_of_v<
        ::jxx::lang::Object,
        ::jxx::net::SocketOptions>));
}

TEST(SocketOptionParityTest, StandardNamesAreCanonical) {
    EXPECT_EQ(
        ::jxx::net::StandardSocketOptions::SO_KEEPALIVE_->name()->utf8(),
        "SO_KEEPALIVE");
    EXPECT_EQ(
        ::jxx::net::StandardSocketOptions::TCP_NODELAY_->name()->utf8(),
        "TCP_NODELAY");
}

TEST(SocketOptionParityTest, LegacyOptionIdsMatchJava8) {
    EXPECT_EQ(::jxx::net::SocketOptions::TCP_NODELAY_, 0x0001);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_TIMEOUT_, 0x1006);
    EXPECT_EQ(::jxx::net::SocketOptions::SO_RCVBUF_, 0x1002);
}

} // namespace
