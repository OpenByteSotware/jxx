#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
namespace {
TEST(SocketTimeoutConnectionStateParityTest, OverridesSocketStateAndTimeoutSurface) {
    using S = ::jxx::ext::net::ssl::internal::OpenSslSocket;
    using ConnectWithTimeout = void (S::*)(
        const ::jxx::Ptr<::jxx::net::SocketAddress>&,
        ::jxx::lang::jint);
    const ConnectWithTimeout connectMethod = &S::connect;
    EXPECT_NE(connectMethod, nullptr);
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&S::setSoTimeout)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&S::getSoTimeout)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&S::isConnected)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&S::isClosed)>));
}
}
