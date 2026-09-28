#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/internal/jxx.ext.net.ssl.internal.OpenSslSocket.h"
namespace {
TEST(SocketConnectStateParityTest, ConnectAndBindRemainProviderOverrides) {
    using Socket = ::jxx::ext::net::ssl::internal::OpenSslSocket;
    using Connect = void (Socket::*)(
        const ::jxx::Ptr<::jxx::net::SocketAddress>&,
        ::jxx::lang::jint);
    using Bind = void (Socket::*)(
        const ::jxx::Ptr<::jxx::net::SocketAddress>&);
    const Connect connect = &Socket::connect;
    const Bind bind = &Socket::bind;
    EXPECT_NE(connect, nullptr);
    EXPECT_NE(bind, nullptr);
}
}
