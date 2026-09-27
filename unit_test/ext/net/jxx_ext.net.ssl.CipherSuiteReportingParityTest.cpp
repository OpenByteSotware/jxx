#include <gtest/gtest.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslCipherSuites.h"

namespace {

TEST(CipherSuiteReportingParityTest, ClientSuitesAreReported) {
    const auto defaults =
        ::jxx::ext::net::ssl::internal::
            clientDefaultCipherSuites();
    const auto supported =
        ::jxx::ext::net::ssl::internal::
            clientSupportedCipherSuites();

    ASSERT_NE(defaults, nullptr);
    ASSERT_NE(supported, nullptr);
    EXPECT_GT(defaults->length, 0);
    EXPECT_GT(supported->length, 0);
}

TEST(CipherSuiteReportingParityTest, ServerSuitesAreReported) {
    const auto defaults =
        ::jxx::ext::net::ssl::internal::
            serverDefaultCipherSuites();
    const auto supported =
        ::jxx::ext::net::ssl::internal::
            serverSupportedCipherSuites();

    ASSERT_NE(defaults, nullptr);
    ASSERT_NE(supported, nullptr);
    EXPECT_GT(defaults->length, 0);
    EXPECT_GT(supported->length, 0);
}

} // namespace
