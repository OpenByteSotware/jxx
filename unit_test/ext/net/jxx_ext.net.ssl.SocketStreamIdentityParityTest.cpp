#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
namespace {
TEST(SocketStreamIdentityParityTest, StreamGetterSurfacesRemainAvailable) {
 EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&::jxx::ext::net::ssl::internal::OpenSslSocket::getInputStream)>));
 EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&::jxx::ext::net::ssl::internal::OpenSslSocket::getOutputStream)>));
}
}
