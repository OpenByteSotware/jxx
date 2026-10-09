#include <gtest/gtest.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {

class SystemPropertyGuard final {
public:
    explicit SystemPropertyGuard(const char* name)
        : name_(::jxx::NEW<::jxx::lang::String>(name)),
          previous_(::jxx::lang::System::getProperty(name_)) {
    }

    ~SystemPropertyGuard() {
        try {
            if (previous_ == nullptr) {
                (void)::jxx::lang::System::clearProperty(name_);
            }
            else {
                (void)::jxx::lang::System::setProperty(name_, previous_);
            }
        }
        catch (...) {
        }
    }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> previous_;
};

TEST(
    SSLContextDefaultTrustStoreParity,
    ExplicitNoneProducesEmptyTrustConfiguration)
{
    SystemPropertyGuard trustStore(
        "jxx.ext.net.ssl.trustStore");

    SystemPropertyGuard password(
        "jxx.ext.net.ssl.trustStorePassword");

    const auto trustStoreKey =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStore");

    const auto trustStoreValue =
        ::jxx::NEW<::jxx::lang::String>(
            "NONE");

    (void)::jxx::lang::System::setProperty(
        trustStoreKey,
        trustStoreValue);

    (void)::jxx::lang::System::clearProperty(
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStorePassword"));

    const auto configuredValue =
        ::jxx::lang::System::getProperty(
            trustStoreKey);

    ASSERT_NE(nullptr, configuredValue);
    ASSERT_EQ("NONE", configuredValue->utf8());

    const auto managers =
        ::jxx::ext::net::ssl::internal::
        loadDefaultPropertyTrustManagers();

    ASSERT_NE(nullptr, managers);
    ASSERT_EQ(1U, managers->length);
    ASSERT_NE(nullptr, (*managers)[0]);
}

TEST(SSLContextDefaultTrustStoreParity, AbsentTrustStoreUsesDefaultTrustPath) {
    SystemPropertyGuard trustStore("javax.net.ssl.trustStore");
    SystemPropertyGuard password("javax.net.ssl.trustStorePassword");

    (void)::jxx::lang::System::clearProperty(
        ::jxx::NEW<::jxx::lang::String>("javax.net.ssl.trustStore"));
    (void)::jxx::lang::System::clearProperty(
        ::jxx::NEW<::jxx::lang::String>(
            "javax.net.ssl.trustStorePassword"));

    EXPECT_EQ(
        nullptr,
        ::jxx::ext::net::ssl::internal::
            loadDefaultPropertyTrustManagers());
}

TEST(SSLContextDefaultTrustStoreParity, MissingConfiguredStoreProducesEmptyTrustConfiguration)
{
    SystemPropertyGuard trustStore(
        "jxx.ext.net.ssl.trustStore");

    SystemPropertyGuard password(
        "jxx.ext.net.ssl.trustStorePassword");

    const auto trustStoreKey =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStore");

    const auto passwordKey =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStorePassword");

    (void)::jxx::lang::System::setProperty(
        trustStoreKey,
        ::jxx::NEW<::jxx::lang::String>(
            "jxx-test-missing-trust-store-8d729a.pem"));

    (void)::jxx::lang::System::setProperty(
        passwordKey,
        ::jxx::NEW<::jxx::lang::String>(
            "changeit"));

    const auto configuredTrustStore =
        ::jxx::lang::System::getProperty(
            trustStoreKey);

    ASSERT_NE(nullptr, configuredTrustStore);
    EXPECT_EQ(
        "jxx-test-missing-trust-store-8d729a.pem",
        configuredTrustStore->utf8());

    const auto managers =
        ::jxx::ext::net::ssl::internal::
        loadDefaultPropertyTrustManagers();

    ASSERT_NE(nullptr, managers);
    ASSERT_EQ(1U, managers->length);
    EXPECT_NE(nullptr, (*managers)[0]);
}

TEST(SSLContextDefaultTrustStoreParity, DefaultContextStillCreatesFactory) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getDefault();
    ASSERT_NE(nullptr, context);
    EXPECT_NE(nullptr, context->getSocketFactory());
}

} // namespace
