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
 const auto start=std::chrono::steady_clock::now();
 EXPECT_THROW((void)server->accept(),::jxx::net::SocketTimeoutException);
 const auto elapsed=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-start).count();
 EXPECT_GE(elapsed,50);
 EXPECT_LT(elapsed,2000);
 server->close();
}
}
