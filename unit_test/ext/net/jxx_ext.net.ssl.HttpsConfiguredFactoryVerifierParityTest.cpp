#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
namespace {
TEST(HttpsConfiguredFactoryVerifierParityTest, ConfigurationSurfacesRemainAvailable) {
 EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&::jxx::ext::net::ssl::HttpsURLConnection::getSSLSocketFactory)>));
 EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&::jxx::ext::net::ssl::HttpsURLConnection::getHostnameVerifier)>));
}
}
