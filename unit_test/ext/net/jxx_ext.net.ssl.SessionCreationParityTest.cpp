#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
namespace {
TEST(SessionCreationParityTest, PublicSessionCreationSurfaceRemainsStable) {
    using Socket = ::jxx::ext::net::ssl::SSLSocket;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::setEnableSessionCreation)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::getEnableSessionCreation)>));
}
}
