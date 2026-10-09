#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {
TEST(SSLDefaultTrustStoreParity, MissingConfiguredStoreReturnsEmptyManagerArray) {
    const auto key = ::jxx::NEW<::jxx::lang::String>(
        "jxx.ext.net.ssl.trustStore");
    const auto previous = ::jxx::lang::System::getProperty(key);

    (void)::jxx::lang::System::setProperty(
        key,
        ::jxx::NEW<::jxx::lang::String>(
            "jxx-test-missing-trust-store-value-parity.pem"));

    const auto managers = ::jxx::ext::net::ssl::internal::
        loadDefaultPropertyTrustManagers();

    if (previous == nullptr) {
        (void)::jxx::lang::System::clearProperty(key);
    } else {
        (void)::jxx::lang::System::setProperty(key, previous);
    }

    ASSERT_NE(nullptr, managers);
    ASSERT_EQ(1U, managers->length);
    EXPECT_NE(nullptr, (*managers)[0]);
}
} // namespace
