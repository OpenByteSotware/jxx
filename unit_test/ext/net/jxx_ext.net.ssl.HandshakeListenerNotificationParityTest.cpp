#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "ext/net/ssl/jxx.ext.net.ssl.SSLSocket.h"
namespace {
TEST(HandshakeListenerNotificationParityTest, PublicCallbackSurfaceIsStable) {
    using Listener = ::jxx::ext::net::ssl::HandshakeCompletedListener;
    using Socket = ::jxx::ext::net::ssl::SSLSocket;
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Listener::handshakeCompleted)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::addHandshakeCompletedListener)>));
    EXPECT_TRUE((std::is_member_function_pointer_v<
        decltype(&Socket::removeHandshakeCompletedListener)>));
}
}
