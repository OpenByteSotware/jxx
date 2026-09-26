#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSessionContext.h"

TEST(NativeSessionCachePolicyTest, AcceptsZeroUnlimitedPolicies) {
    const auto context = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslSessionContext>();
    context->setSessionTimeout(0);
    context->setSessionCacheSize(0);
    EXPECT_EQ(context->getSessionTimeout(), 0);
    EXPECT_EQ(context->getSessionCacheSize(), 0);
}
