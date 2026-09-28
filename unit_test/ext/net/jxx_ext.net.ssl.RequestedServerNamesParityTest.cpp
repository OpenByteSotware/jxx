#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
namespace {
TEST(RequestedServerNamesParityTest, EmptySessionReturnsIndependentEmptyList) {
    const auto session = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslSession>(
            ::jxx::NEW<::jxx::lang::String>("TLS_AES_128_GCM_SHA256"),
            ::jxx::NEW<::jxx::lang::String>("TLSv1.3"),
            nullptr,
            -1);
    const auto first = session->getRequestedServerNames();
    const auto second = session->getRequestedServerNames();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
    EXPECT_TRUE(first->isEmpty());
}
}
