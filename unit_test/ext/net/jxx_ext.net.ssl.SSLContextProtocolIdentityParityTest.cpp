#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslProtocolPolicy.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
namespace {
TEST(SSLContextProtocolIdentityParityTest, VersionNamesUseExactNativeRanges) {
    using ::jxx::ext::net::ssl::internal::protocolRange;
    const auto tls10 = protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1"));
    const auto tls11 = protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1.1"));
    const auto tls12 = protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1.2"));
    const auto tls13 = protocolRange(::jxx::NEW<::jxx::lang::String>("TLSv1.3"));
    EXPECT_EQ(tls10.first, tls10.second);
    EXPECT_EQ(tls11.first, tls11.second);
    EXPECT_EQ(tls12.first, tls12.second);
    EXPECT_EQ(tls13.first, tls13.second);
    EXPECT_LT(tls10.first, tls11.first);
    EXPECT_LT(tls11.first, tls12.first);
    EXPECT_LT(tls12.first, tls13.first);
}
TEST(SSLContextProtocolIdentityParityTest, Tls11ContextPreservesProtocolName) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLSv1.1"));
    ASSERT_NE(context, nullptr);
    EXPECT_EQ(context->getProtocol()->utf8(), "TLSv1.1");
}
}
