#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketAddress.h"
#include "net/jxx.net.SocketException.h"

namespace {
class UnsupportedSocketAddress final : public ::jxx::net::SocketAddress {
};
}

TEST(SocketArgumentParityTest, RejectsInvalidConnectAndBindArguments) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->connect(nullptr, 0), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->connect(::jxx::NEW<UnsupportedSocketAddress>(), -1),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->bind(::jxx::NEW<UnsupportedSocketAddress>()),
        ::jxx::lang::IllegalArgumentException);
}

TEST(SocketOptionParityTest, BufferSizesMustBePositive) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->setSendBufferSize(0), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->setReceiveBufferSize(-1), ::jxx::lang::IllegalArgumentException);

    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>();
    EXPECT_THROW(server->setReceiveBufferSize(0), ::jxx::lang::IllegalArgumentException);
}

TEST(ServerSocketLifecycleParityTest, ClosedSocketCannotBind) {
    const auto server = ::jxx::NEW<::jxx::net::ServerSocket>();
    server->close();
    EXPECT_THROW(server->bind(nullptr), ::jxx::net::SocketException);
}
