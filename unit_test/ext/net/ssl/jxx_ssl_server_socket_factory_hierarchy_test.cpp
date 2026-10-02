#include <gtest/gtest.h>

#include "ext/net/jxx.ext.net.ServerSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "net/jxx.net.ServerSocket.h"

TEST(SSLServerSocketFactoryHierarchyTest, DefaultFactoryUsesSuperclassApi) {
    const ::jxx::Ptr<::jxx::ext::net::ServerSocketFactory> factory =
        ::jxx::ext::net::ssl::SSLServerSocketFactory::getDefault();
    ASSERT_NE(nullptr, factory);

    const auto sslFactory = ::jxx::CAST<
        ::jxx::ext::net::ssl::SSLServerSocketFactory>(factory);
    ASSERT_NE(nullptr, sslFactory);
    ASSERT_NE(nullptr, sslFactory->getDefaultCipherSuites());
    ASSERT_NE(nullptr, sslFactory->getSupportedCipherSuites());
}

TEST(SSLServerSocketFactoryHierarchyTest, GeneralDefaultCreatesUnboundSocket) {
    const auto factory =
        ::jxx::ext::net::ServerSocketFactory::getDefault();
    ASSERT_NE(nullptr, factory);
    const auto socket = factory->createServerSocket();
    ASSERT_NE(nullptr, socket);
    EXPECT_FALSE(socket->isBound());
    EXPECT_FALSE(socket->isClosed());
    socket->close();
}
