#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedTrustManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509KeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509TrustManager.h"
namespace {
TEST(ManagerIntegrationParityTest, ExtendedManagersPreserveBaseContracts) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::net::ssl::X509KeyManager,
        ::jxx::ext::net::ssl::X509ExtendedKeyManager>));
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::net::ssl::X509TrustManager,
        ::jxx::ext::net::ssl::X509ExtendedTrustManager>));
}
}
