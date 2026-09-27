#include <gtest/gtest.h>
#include <openssl/ssl.h>
namespace {
TEST(BlockingRetryParityTest, OpenSslExposesRetryStates) {
    EXPECT_NE(SSL_ERROR_WANT_READ, SSL_ERROR_NONE);
    EXPECT_NE(SSL_ERROR_WANT_WRITE, SSL_ERROR_NONE);
    EXPECT_NE(SSL_ERROR_WANT_READ, SSL_ERROR_WANT_WRITE);
}
}
