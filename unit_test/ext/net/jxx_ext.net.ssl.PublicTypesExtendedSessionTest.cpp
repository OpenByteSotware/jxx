#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.ExtendedSSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.ManagerFactoryParameters.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLPermission.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
TEST(PublicTypesExtendedSessionTest, OpenSslSessionIsExtendedSession) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::net::ssl::ExtendedSSLSession,
        ::jxx::ext::net::ssl::internal::OpenSslSession>));
}
TEST(PublicTypesExtendedSessionTest, SSLPermissionConstructs) {
    const auto permission = ::jxx::NEW<::jxx::ext::net::ssl::SSLPermission>(
        ::jxx::NEW<::jxx::lang::String>("setHostnameVerifier"));
    ASSERT_NE(permission, nullptr);
}
