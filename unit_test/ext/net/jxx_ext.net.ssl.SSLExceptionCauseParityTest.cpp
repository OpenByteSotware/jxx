#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(
    SSLExceptionCauseParityTest,
    CauseConstructorRetainsCause)
{
    const auto cause =
        ::jxx::NEW<
            ::jxx::lang::Exception>(
                ::jxx::NEW<
                    ::jxx::lang::String>(
                        "cause"));

    const auto exception =
        ::jxx::NEW<
            ::jxx::ext::net::ssl::
                SSLException>(
                    cause);

    EXPECT_EQ(
        exception->getCause(),
        cause);

    EXPECT_NE(
        exception->getMessage(),
        nullptr);
}

TEST(
    SSLExceptionCauseParityTest,
    MessageAndCauseConstructorRetainsBoth)
{
    const auto cause =
        ::jxx::NEW<
            ::jxx::lang::Exception>();

    const auto message =
        ::jxx::NEW<
            ::jxx::lang::String>(
                "TLS failure");

    const auto exception =
        ::jxx::NEW<
            ::jxx::ext::net::ssl::
                SSLException>(
                    message,
                    cause);

    EXPECT_EQ(
        exception->getMessage(),
        message);

    EXPECT_EQ(
        exception->getCause(),
        cause);
}

} // namespace
