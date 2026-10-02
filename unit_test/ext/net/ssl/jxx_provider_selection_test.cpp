#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.KeyManagerFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.TrustManagerFactory.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
#include "security/jxx.security.Provider.h"

namespace {
::jxx::Ptr<::jxx::lang::String> text(const char* value) {
    return ::jxx::NEW<::jxx::lang::String>(value);
}
}

TEST(ProviderSelectionTest, UnknownProviderNameThrowsNoSuchProvider) {
    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(text("TLS"), text("missing")),
        ::jxx::security::NoSuchProviderException);
    EXPECT_THROW(
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(text("SunX509"), text("missing")),
        ::jxx::security::NoSuchProviderException);
    EXPECT_THROW(
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(text("PKIX"), text("missing")),
        ::jxx::security::NoSuchProviderException);
}

TEST(ProviderSelectionTest, EmptyProviderNameThrowsIllegalArgument) {
    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(text("TLS"), text("")),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(text("SunX509"), text("")),
        ::jxx::lang::IllegalArgumentException);
}

TEST(ProviderSelectionTest, ProviderObjectIsPreserved) {
    const auto provider = ::jxx::NEW<::jxx::security::Provider>(text("OpenSSL"));
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(text("TLS"), provider);
    const auto keyFactory = ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(text("SunX509"), provider);
    const auto trustFactory = ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(text("PKIX"), provider);
    EXPECT_EQ(provider, context->getProvider());
    EXPECT_EQ(provider, keyFactory->getProvider());
    EXPECT_EQ(provider, trustFactory->getProvider());
}

TEST(ProviderSelectionTest, UnsupportedAlgorithmThrowsNoSuchAlgorithm) {
    EXPECT_THROW(
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(text("missing")),
        ::jxx::security::NoSuchAlgorithmException);
    EXPECT_THROW(
        ::jxx::ext::net::ssl::TrustManagerFactory::getInstance(text("missing")),
        ::jxx::security::NoSuchAlgorithmException);
}

TEST(ProviderSelectionTest, ProviderObjectWithoutServiceThrowsNoSuchAlgorithm) {
    const auto provider = ::jxx::NEW<::jxx::security::Provider>(text("Other"));
    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(text("TLS"), provider),
        ::jxx::security::NoSuchAlgorithmException);
    EXPECT_THROW(
        ::jxx::ext::net::ssl::KeyManagerFactory::getInstance(text("SunX509"), provider),
        ::jxx::security::NoSuchAlgorithmException);
}
