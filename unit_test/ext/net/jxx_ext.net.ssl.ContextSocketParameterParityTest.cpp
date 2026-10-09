#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "lang/jxx.lang.String.h"

namespace
{
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext>
        createInitializedTlsContext()
    {
        const auto context =
            ::jxx::ext::net::ssl::SSLContext::getInstance(
                ::jxx::NEW<::jxx::lang::String>("TLS"));

        context->init(nullptr, nullptr, nullptr);

        return context;
    }

    TEST(
        ContextSocketParameterParityTest,
        ContextParametersContainProtocols)
    {
        const auto context =
            createInitializedTlsContext();

        const auto defaults =
            context->getDefaultSSLParameters();

        const auto supported =
            context->getSupportedSSLParameters();

        ASSERT_NE(nullptr, defaults);
        ASSERT_NE(nullptr, supported);

        const auto defaultProtocols =
            defaults->getProtocols();

        const auto supportedProtocols =
            supported->getProtocols();

        ASSERT_NE(nullptr, defaultProtocols);
        ASSERT_NE(nullptr, supportedProtocols);

        EXPECT_GT(defaultProtocols->length, 0U);
        EXPECT_GT(supportedProtocols->length, 0U);
    }

    TEST(
        ContextSocketParameterParityTest,
        SocketAdvertisesCiphers)
    {
        const auto context =
            createInitializedTlsContext();

        const auto factory =
            context->getSocketFactory();

        ASSERT_NE(nullptr, factory);

        const auto socketFactory =
            ::jxx::CAST<::jxx::ext::net::SocketFactory>(
                factory);

        ASSERT_NE(nullptr, socketFactory);

        const auto socket =
            ::jxx::CAST<::jxx::ext::net::ssl::SSLSocket>(
                socketFactory->createSocket());

        ASSERT_NE(nullptr, socket);

        const auto supportedCipherSuites =
            socket->getSupportedCipherSuites();

        ASSERT_NE(nullptr, supportedCipherSuites);
        EXPECT_GT(supportedCipherSuites->length, 0U);
    }

} // namespace