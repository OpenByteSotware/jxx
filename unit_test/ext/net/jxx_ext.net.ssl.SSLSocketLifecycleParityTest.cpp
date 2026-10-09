#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
namespace
{
    TEST(SSLSocketLifecycleParityTest, NullListenerOperationsThrow)
    {
        const auto context =
            ::jxx::ext::net::ssl::SSLContext::getInstance(
                ::jxx::NEW<::jxx::lang::String>("TLS"));

        context->init(nullptr, nullptr, nullptr);

        const auto factory =
            context->getSocketFactory();

        const auto baseFactory =
            ::jxx::CAST<::jxx::ext::net::SocketFactory>(
                factory);

        const auto socket =
            ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(
                baseFactory->createSocket());

        ASSERT_NE(nullptr, socket);

        EXPECT_THROW(
            socket->addHandshakeCompletedListener(nullptr),
            ::jxx::lang::IllegalArgumentException);

        EXPECT_THROW(
            socket->removeHandshakeCompletedListener(nullptr),
            ::jxx::lang::IllegalArgumentException);
    }
}
