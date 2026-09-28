#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSniMatcher.h"
TEST(SniMatcherEnforcementParityTest, CallbackIsAvailable) {
    EXPECT_NE(&::jxx::ext::net::ssl::internal::openSslServerNameMatcherCallback,nullptr);
}
