#include <gtest/gtest.h>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLContext.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocketFactory.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
namespace {
TEST(LayeredFactoryJava8ParityTest, NullUnderlyingSocketThrowsNullPointer) {
    const auto context = ::jxx::ext::net::ssl::SSLContext::getInstance(
        ::jxx::NEW<::jxx::lang::String>("TLS"));
    const auto factory = context->getSocketFactory();
    ASSERT_NE(factory, nullptr);
    EXPECT_THROW(factory->createSocket(
        nullptr,
        ::jxx::Ptr<::jxx::io::InputStream>(),
        true),
        ::jxx::lang::NullPointerException);
}
}
