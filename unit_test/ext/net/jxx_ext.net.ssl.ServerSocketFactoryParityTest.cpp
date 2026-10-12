#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "net/jxx.net.ServerSocket.h"

TEST(ServerSocketFactoryParityTest, AdvertisesCipherSuites)
{
    const auto context =
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            ::jxx::NEW<::jxx::lang::String>("TLS"));

    context->init(nullptr, nullptr, nullptr);

    const auto factory =
        context->getServerSocketFactory();

    ASSERT_NE(nullptr, factory);

    const auto supported =
        factory->getSupportedCipherSuites();

    ASSERT_NE(nullptr, supported);
    EXPECT_GT(supported->length, 0);
}

TEST(ServerSocketFactoryParityTest, ZeroBacklogUsesImplementationDefault)
{
    const auto context =
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            ::jxx::NEW<::jxx::lang::String>("TLS"));

    context->init(nullptr, nullptr, nullptr);

    const auto factory =
        context->getServerSocketFactory();

    ASSERT_NE(nullptr, factory);

    const auto socket =  factory->createServerSocket(0, 0);

    ASSERT_NE(nullptr, socket);
    EXPECT_TRUE(socket->isBound());
    EXPECT_NE(-1, socket->getLocalPort());

    socket->close();
}