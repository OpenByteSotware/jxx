#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(ContextProtocolPolicyTest, VersionSpecificContextsAreRestricted) {
    const auto tls12 = ::jxx::ext::net::ssl::internal::contextProtocols(
        ::jxx::NEW<::jxx::lang::String>("TLSv1.2"));
    ASSERT_NE(tls12, nullptr);
    ASSERT_EQ(tls12->length, 1);
    EXPECT_EQ((*tls12)[0]->utf8(), "TLSv1.2");
    const auto tls13 = ::jxx::ext::net::ssl::internal::contextProtocols(
        ::jxx::NEW<::jxx::lang::String>("TLSv1.3"));
    ASSERT_EQ(tls13->length, 1);
    EXPECT_EQ((*tls13)[0]->utf8(), "TLSv1.3");
}
}
