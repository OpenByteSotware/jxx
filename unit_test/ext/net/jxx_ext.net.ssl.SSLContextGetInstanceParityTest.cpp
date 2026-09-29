#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "security/jxx.security.NoSuchAlgorithmException.h"

namespace {

TEST(
    SSLContextGetInstanceParityTest,
    TlsCreatesUninitializedContextWithRequestedProtocol)
{
    const auto context =
        ::jxx::ext::net::ssl::
            SSLContext::getInstance(
                ::jxx::NEW<
                    ::jxx::lang::String>(
                        "TLS"));

    ASSERT_NE(context, nullptr);
    ASSERT_NE(context->getProtocol(), nullptr);
    EXPECT_EQ(
        context->getProtocol()->utf8(),
        "TLS");
}

TEST(
    SSLContextGetInstanceParityTest,
    DefaultReturnsProcessDefaultContext)
{
    const auto byName =
        ::jxx::ext::net::ssl::
            SSLContext::getInstance(
                ::jxx::NEW<
                    ::jxx::lang::String>(
                        "Default"));

    EXPECT_EQ(
        byName,
        ::jxx::ext::net::ssl::
            SSLContext::getDefault());
}

TEST(
    SSLContextGetInstanceParityTest,
    RejectsNullAndUnsupportedProtocol)
{
    EXPECT_THROW(
        ::jxx::ext::net::ssl::
            SSLContext::getInstance(
                nullptr),
        ::jxx::lang::
            NullPointerException);

    EXPECT_THROW(
        ::jxx::ext::net::ssl::
            SSLContext::getInstance(
                ::jxx::NEW<
                    ::jxx::lang::String>(
                        "NOT_TLS")),
        ::jxx::security::
            NoSuchAlgorithmException);
}

} // namespace
