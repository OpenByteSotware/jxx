#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace {
using namespace jxx::ext::net::ssl;
/*
TEST(SSLSocketParityTest, UnconnectedSocketAdvertisesCipherSuites) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto socket = ::jxx::CAST<SSLSocket>(
        context->getSocketFactory()->createSocket());
    ASSERT_NE(socket, nullptr);
    const auto suites = socket->getSupportedCipherSuites();
    ASSERT_NE(suites, nullptr);
    EXPECT_GT(suites->length, 0);
}

TEST(SSLSocketParityTest, RemovingNullHandshakeListenerThrows) {
    const auto context = SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto socket = ::jxx::CAST<SSLSocket>(
        context->getSocketFactory()->createSocket());
    ASSERT_NE(socket, nullptr);
    EXPECT_THROW(
        socket->removeHandshakeCompletedListener(nullptr),
        ::jxx::lang::IllegalArgumentException);
}
*/
} // namespace
