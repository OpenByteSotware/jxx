#include <gtest/gtest.h>
#include "com/sun/net/httpserver/internal/jxx.com.sun.net.httpserver.internal.DefaultHttpsParameters.h"
#include "com/sun/net/httpserver/jxx.com.sun.net.httpserver.HttpsConfigurator.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLParameters.h"
#include "lang/jxx.lang.String.h"
#include "net/jxx.net.InetSocketAddress.h"

namespace jxx::com::sun::net::httpserver::internal {

TEST(ServerConformanceTests, SetSslParametersNullLeavesExistingSnapshotUnchanged)
{
    auto context = ::jxx::ext::net::ssl::SSLContext::getDefault();
    auto configurator = ::jxx::NEW<::jxx::com::sun::net::httpserver::HttpsConfigurator>(context);
    auto address = ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"), 1);
    auto parameters = ::jxx::NEW<DefaultHttpsParameters>(address, configurator);
    auto aggregate = context->getDefaultSSLParameters();
    parameters->setSSLParameters(aggregate);
    parameters->setSSLParameters(nullptr);
    EXPECT_NE(parameters->getAppliedSSLParameters(), nullptr);
}

TEST(ServerConformanceTests, HttpsParametersExposeOriginalConfiguratorAndClientAddress)
{
    auto context = ::jxx::ext::net::ssl::SSLContext::getDefault();
    auto configurator = ::jxx::NEW<::jxx::com::sun::net::httpserver::HttpsConfigurator>(context);
    auto address = ::jxx::NEW<::jxx::net::InetSocketAddress>(::jxx::NEW<::jxx::lang::String>("127.0.0.1"), 2);
    auto parameters = ::jxx::NEW<DefaultHttpsParameters>(address, configurator);
    EXPECT_EQ(parameters->getHttpsConfigurator().get(), configurator.get());
    EXPECT_EQ(parameters->getClientAddress().get(), address.get());
}

} // namespace jxx::com::sun::net::httpserver::internal
