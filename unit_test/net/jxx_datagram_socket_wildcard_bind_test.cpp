#include <gtest/gtest.h>

#include "net/jxx.net.DatagramSocket.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace {

TEST(DatagramSocketApiParity, WildcardEphemeralBindAvoidsNameResolution) {
    const auto socket = ::jxx::NEW<::jxx::net::DatagramSocket>();

    socket->bind(
        ::jxx::NEW<::jxx::net::InetSocketAddress>(0));

    EXPECT_TRUE(socket->isBound());
    EXPECT_NE(-1, socket->getLocalPort());
    EXPECT_NE(nullptr, socket->getLocalSocketAddress());
    EXPECT_EQ(nullptr, socket->getChannel());

    socket->close();
}

} // namespace
