#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLHandshakeException.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLProtocolException.h"
#include "io/jxx.io.IOException.h"
namespace {
TEST(SocketFailureExceptionParityTest, SslExceptionsRemainIoExceptions) {
    EXPECT_TRUE((std::is_base_of_v<::jxx::io::IOException, ::jxx::ext::net::ssl::SSLException>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::ext::net::ssl::SSLException, ::jxx::ext::net::ssl::SSLHandshakeException>));
    EXPECT_TRUE((std::is_base_of_v<::jxx::ext::net::ssl::SSLException, ::jxx::ext::net::ssl::SSLProtocolException>));
}
}
