#include <gtest/gtest.h>
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.Socket.h"
#include "net/jxx.net.SocketImplFactory.h"
#include "lang/jxx.lang.NullPointerException.h"

TEST(SocketImplFactoryTest, NullFactoryIsRejected) {
    EXPECT_THROW(
        ::jxx::net::Socket::setSocketImplFactory(nullptr),
        ::jxx::lang::NullPointerException);
}

TEST(SocketImplFactoryTest, NullServerFactoryIsRejected) {
    EXPECT_THROW(
        ::jxx::net::ServerSocket::setSocketFactory(nullptr),
        ::jxx::lang::NullPointerException);
}
