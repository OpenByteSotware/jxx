#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocketFactory.h"
namespace {
TEST(ConsumedInputNonBlockingParityTest, ConsumedInputOverloadRemainsImplemented) {
    using Factory = ::jxx::ext::net::ssl::internal::OpenSslSocketFactory;
    using Method = ::jxx::Ptr<::jxx::net::Socket> (Factory::*)(
        const ::jxx::Ptr<::jxx::net::Socket>&,
        const ::jxx::Ptr<::jxx::io::InputStream>&,
        ::jxx::lang::jbool);
    const Method method = &Factory::createSocket;
    EXPECT_NE(method, nullptr);
}
}
