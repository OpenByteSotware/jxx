#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
TEST(ServerSocketFactoryParityTest, AdvertisesCipherSuites) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto factory = context->getServerSocketFactory();
    ASSERT_NE(factory->getSupportedCipherSuites(), nullptr);
    EXPECT_GT(factory->getSupportedCipherSuites()->length, 0);
}
TEST(ServerSocketFactoryParityTest, RejectsInvalidBacklog) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(::jxx::NEW<::jxx::lang::String>("TLS"));
    EXPECT_THROW(context->getServerSocketFactory()->createServerSocket(0, 0), ::jxx::lang::IllegalArgumentException);
}
