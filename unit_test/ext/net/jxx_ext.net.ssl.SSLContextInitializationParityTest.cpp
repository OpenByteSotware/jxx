#include <gtest/gtest.h>

#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(SSLContextInitializationParityTest, NewContextRequiresInitialization) {
    const auto context =
        ::jxx::ext::net::ssl::SSLContext::getInstance(
            ::jxx::NEW<::jxx::lang::String>("TLS"));

    EXPECT_THROW(
        context->getSocketFactory(),
        ::jxx::lang::IllegalStateException);

    context->init(nullptr, nullptr, nullptr);

    EXPECT_NE(context->getSocketFactory(), nullptr);
    EXPECT_NE(context->getServerSocketFactory(), nullptr);
    EXPECT_NE(context->createSSLEngine(), nullptr);
}

TEST(SSLContextInitializationParityTest, DefaultContextIsInitialized) {
    const auto context =
        ::jxx::ext::net::ssl::SSLContext::getDefault();
    EXPECT_NE(context->getSocketFactory(), nullptr);
}

} // namespace
