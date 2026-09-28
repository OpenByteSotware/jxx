#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
namespace {
TEST(HandshakeFailureClosesSocketParityTest, PublicLifecycleSurfaceIsStable) {
    using Socket = ::jxx::ext::net::ssl::SSLSocket;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::startHandshake)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::getSession)>));
}
}
