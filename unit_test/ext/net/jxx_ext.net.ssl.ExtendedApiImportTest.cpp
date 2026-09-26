#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.ExtendedSSLSession.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SNIMatcher.h"
#include "ext/net/ssl/jxx.ext.net.ssl.StandardConstants.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedKeyManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.X509ExtendedTrustManager.h"
TEST(ExtendedApiImportTest, StandardSniHostNameTypeIsZero) {
    EXPECT_EQ(::jxx::ext::net::ssl::StandardConstants::SNI_HOST_NAME, 0);
}
