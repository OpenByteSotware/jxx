#include <gtest/gtest.h>

#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLServerSocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "lang/jxx.lang.String.h"

namespace {

    ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext> initializedTlsContext()
    {
        const auto context =
            ::jxx::ext::net::ssl::SSLContext::getInstance(
                ::jxx::NEW<::jxx::lang::String>("TLS"));

        context->init(nullptr, nullptr, nullptr);
        return context;
    }

TEST(P0RuntimeReportingTest, ClientFactoryReportsRuntimeCipherSuites) {
    const auto context = initializedTlsContext();
    const auto factory = context->getSocketFactory();
    ASSERT_NE(factory, nullptr);
    const auto defaults = factory->getDefaultCipherSuites();
    const auto supported = factory->getSupportedCipherSuites();
    ASSERT_NE(defaults, nullptr);
    ASSERT_NE(supported, nullptr);
    EXPECT_GT(defaults->length, 0);
    EXPECT_GT(supported->length, 0);
}

TEST(P0RuntimeReportingTest, ServerFactoryReportsRuntimeCipherSuites) {
    const auto context = initializedTlsContext();
    const auto factory = context->getServerSocketFactory();
    ASSERT_NE(factory, nullptr);
    const auto defaults = factory->getDefaultCipherSuites();
    const auto supported = factory->getSupportedCipherSuites();
    ASSERT_NE(defaults, nullptr);
    ASSERT_NE(supported, nullptr);
    EXPECT_GT(defaults->length, 0);
    EXPECT_GT(supported->length, 0);
}

TEST(P0RuntimeReportingTest, DefaultSocketFactoryIsAvailable) {
    EXPECT_NE(
        ::jxx::ext::net::SocketFactory::getDefault(),
        nullptr);
}

} // namespace
