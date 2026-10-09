#include <type_traits>

#include <gtest/gtest.h>

#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslPropertyTrustManager.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace {

using TrustManagerArrayPtr =
    ::jxx::Ptr<::jxx::ext::net::ssl::SSLContext::TrustManagerArray>;

static_assert(
    std::is_same_v<
        decltype(
            ::jxx::ext::net::ssl::internal::
                loadDefaultPropertyTrustManagers()),
        TrustManagerArrayPtr>,
    "The default trust-store loader must retain its JXX return type");

class SystemPropertyGuard final {
public:
    explicit SystemPropertyGuard(const char* propertyName)
        : name_(::jxx::NEW<::jxx::lang::String>(propertyName)),
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
            // Test cleanup must not terminate the test executable.
        }
    }

private:
    ::jxx::Ptr<::jxx::lang::String> name_;
    ::jxx::Ptr<::jxx::lang::String> previous_;
};

TEST(DefaultTrustStoreApiParity, TrustStorePropertiesRoundTripSafely) {
    SystemPropertyGuard trustStore("jxx.ext.net.ssl.trustStore");
    SystemPropertyGuard trustStorePassword(
        "jxx.ext.net.ssl.trustStorePassword");

    const auto trustStoreName =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStore");
    const auto trustStorePasswordName =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStorePassword");
    const auto trustStoreValue =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx-test-trust-store.p12");
    const auto passwordValue =
        ::jxx::NEW<::jxx::lang::String>("changeit");

    (void)::jxx::lang::System::setProperty(
        trustStoreName,
        trustStoreValue);
    (void)::jxx::lang::System::setProperty(
        trustStorePasswordName,
        passwordValue);

    const auto actualStore =
        ::jxx::lang::System::getProperty(trustStoreName);
    const auto actualPassword =
        ::jxx::lang::System::getProperty(trustStorePasswordName);

    ASSERT_NE(nullptr, actualStore);
    ASSERT_NE(nullptr, actualPassword);
    EXPECT_TRUE(actualStore->equals(trustStoreValue));
    EXPECT_TRUE(actualPassword->equals(passwordValue));
}

TEST(DefaultTrustStoreApiParity, MissingTrustStorePropertyRemainsAbsent) {
    SystemPropertyGuard trustStore("jxx.ext.net.ssl.trustStore");

    const auto trustStoreName =
        ::jxx::NEW<::jxx::lang::String>(
            "jxx.ext.net.ssl.trustStore");
    (void)::jxx::lang::System::clearProperty(trustStoreName);

    EXPECT_EQ(
        nullptr,
        ::jxx::lang::System::getProperty(trustStoreName));
}

} // namespace
