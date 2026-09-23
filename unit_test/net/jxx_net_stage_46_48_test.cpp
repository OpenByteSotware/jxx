#include <gtest/gtest.h>

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketException.h"

TEST(SocketStreamStateTest, UnconnectedSocketRejectsStreams) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->getInputStream(), ::jxx::net::SocketException);
    EXPECT_THROW(socket->getOutputStream(), ::jxx::net::SocketException);
}

TEST(SocketOptionValidationTest, LingerAndTrafficClassValidateRanges) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->setSoLinger(true, -1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->setTrafficClass(-1), ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(socket->setTrafficClass(256), ::jxx::lang::IllegalArgumentException);
}

TEST(SocketShutdownStateTest, UnconnectedSocketRejectsShutdown) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    EXPECT_THROW(socket->shutdownInput(), ::jxx::net::SocketException);
    EXPECT_THROW(socket->shutdownOutput(), ::jxx::net::SocketException);
}

TEST(SocketShutdownStateTest, ClosedSocketRejectsShutdown) {
    const auto socket = ::jxx::NEW<::jxx::net::Socket>();
    socket->close();
    EXPECT_THROW(socket->shutdownInput(), ::jxx::net::SocketException);
    EXPECT_THROW(socket->shutdownOutput(), ::jxx::net::SocketException);
}
