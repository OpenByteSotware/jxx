#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"

TEST(ContextSocketParameterParityTest, ContextParametersContainProtocols) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto defaults = context->getDefaultSSLParameters();
    const auto supported = context->getSupportedSSLParameters();
    ASSERT_NE(defaults->getProtocols(), nullptr);
    ASSERT_NE(supported->getProtocols(), nullptr);
    EXPECT_GT(defaults->getProtocols()->length, 0);
    EXPECT_GT(supported->getProtocols()->length, 0);
}

TEST(ContextSocketParameterParityTest, SocketAdvertisesCiphers) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const ::jxx::Ptr<::jxx::ext::net::ssl::SSLSocketFactory> factory =
        context->getSocketFactory();
    const ::jxx::Ptr<::jxx::ext::net::SocketFactory> socketFactory =
        ::jxx::CAST<::jxx::ext::net::SocketFactory>(factory);
    ASSERT_NE(socketFactory, nullptr);
    const auto socket = ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(
        socketFactory->createSocket());
    ASSERT_NE(socket, nullptr);
    ASSERT_NE(socket->getSupportedCipherSuites(), nullptr);
    EXPECT_GT(socket->getSupportedCipherSuites()->length, 0);
}
