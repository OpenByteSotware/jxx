#include <gtest/gtest.h>
#include <chrono>
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.ServerSocket.h"
#include "net/jxx.net.SocketTimeoutException.h"
namespace {
TEST(ServerSocketTimeoutTest, RejectsNegativeTimeout) {
    const auto server=::jxx::NEW<::jxx::net::ServerSocket>(0);
    EXPECT_THROW(server->setSoTimeout(-1),::jxx::lang::IllegalArgumentException);
    server->close();
}
TEST(ServerSocketTimeoutTest, AcceptWithoutClientThrowsSocketTimeoutException) {
    const auto server=::jxx::NEW<::jxx::net::ServerSocket>(0);
    server->setSoTimeout(100);
    EXPECT_THROW((void)server->accept(),::jxx::net::SocketTimeoutException);
    server->close();
}
}
