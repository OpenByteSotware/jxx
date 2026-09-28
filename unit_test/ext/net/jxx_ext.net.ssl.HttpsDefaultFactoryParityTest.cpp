#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"
namespace { TEST(HttpsDefaultFactoryParityTest, DefaultFactoryIsNonNullAndStable) { const auto a=::jxx::ext::net::ssl::HttpsURLConnection::getDefaultSSLSocketFactory(); const auto b=::jxx::ext::net::ssl::HttpsURLConnection::getDefaultSSLSocketFactory(); ASSERT_NE(a,nullptr); EXPECT_EQ(a,b); } }
