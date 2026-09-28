#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"
#include "security/jxx.security.NoSuchProviderException.h"
#include "security/jxx.security.Provider.h"

namespace {

TEST(SSLContextProviderValidationTest, RejectsUnsupportedProtocol) {
    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            ::jxx::NEW<::jxx::lang::String>("NOT_TLS")),
        ::jxx::security::NoSuchAlgorithmException);
}

TEST(SSLContextProviderValidationTest, ProviderNameExceptionsMatchOverload) {
    const auto tls = ::jxx::NEW<::jxx::lang::String>("TLS");

    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            tls,
            ::jxx::Ptr<::jxx::lang::String>()),
        ::jxx::lang::IllegalArgumentException);

    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            tls,
            ::jxx::NEW<::jxx::lang::String>("MissingProvider")),
        ::jxx::security::NoSuchProviderException);
}

TEST(SSLContextProviderValidationTest, ProviderObjectRequiresImplementation) {
    const auto tls = ::jxx::NEW<::jxx::lang::String>("TLS");
    const auto provider = ::jxx::NEW<::jxx::security::Provider>(
        ::jxx::NEW<::jxx::lang::String>("OtherProvider"));

    EXPECT_THROW(
        ::jxx::ext::net::ssl::SSLContext::getInstance(tls, provider),
        ::jxx::security::NoSuchAlgorithmException);
}

} // namespace
