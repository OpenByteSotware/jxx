#include <gtest/gtest.h>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslContextSpi.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(ContextSpiInitializationStateTest, OperationalObjectsRequireInit) {
    const auto spi = ::jxx::NEW<
        ::jxx::ext::net::ssl::internal::OpenSslContextSpi>(
            ::jxx::NEW<::jxx::lang::String>("TLS"));
    EXPECT_THROW(spi->engineGetSocketFactory(),
        ::jxx::lang::IllegalStateException);
    EXPECT_THROW(spi->engineCreateSSLEngine(),
        ::jxx::lang::IllegalStateException);
    spi->engineInit(nullptr, nullptr, nullptr);
    EXPECT_NE(spi->engineGetSocketFactory(), nullptr);
    EXPECT_NE(spi->engineCreateSSLEngine(), nullptr);
}
}
