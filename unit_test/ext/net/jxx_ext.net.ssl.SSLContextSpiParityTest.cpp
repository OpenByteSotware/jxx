#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContextSpi.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextSpi.h"
#include "security/jxx.security.GeneralSecurityException.h"
#include "security/jxx.security.KeyManagementException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
namespace {
TEST(SSLContextSpiParityTest, SpiAndSecurityHierarchyArePresent) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::ext::net::ssl::SSLContextSpi,
        ::jxx::ext::net::ssl::internal::OpenSslContextSpi>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::security::GeneralSecurityException,
        ::jxx::security::KeyManagementException>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::security::GeneralSecurityException,
        ::jxx::security::NoSuchAlgorithmException>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::security::GeneralSecurityException,
        ::jxx::security::NoSuchProviderException>));
}
}
