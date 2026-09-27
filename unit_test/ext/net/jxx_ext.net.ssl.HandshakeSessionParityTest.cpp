#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSession.h"
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSession.h"
namespace {
TEST(HandshakeSessionParityTest, TemporarySessionImplementsSSLSession) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::ext::net::ssl::SSLSession,
        ::jxx::ext::net::ssl::internal::OpenSslSession>));
}
}
