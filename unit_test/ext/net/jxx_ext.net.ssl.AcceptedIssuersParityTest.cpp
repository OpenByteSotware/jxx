#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslDefaultTrustManager.h"
namespace {
TEST(AcceptedIssuersParityTest, ReturnsNonNullDefensiveArray) {
    const auto manager = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslDefaultTrustManager>();
    const auto first = manager->getAcceptedIssuers();
    const auto second = manager->getAcceptedIssuers();
    ASSERT_NE(first, nullptr);
    ASSERT_NE(second, nullptr);
    EXPECT_NE(first, second);
}
}
