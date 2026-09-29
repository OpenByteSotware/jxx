#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.HttpsURLConnection.h"

namespace {

TEST(DefaultHostnameVerifierParityTest, DefaultVerifierIsNonNull) {
    EXPECT_NE(
        ::jxx::ext::net::ssl::HttpsURLConnection::
            getDefaultHostnameVerifier(),
        nullptr);
}

} // namespace
