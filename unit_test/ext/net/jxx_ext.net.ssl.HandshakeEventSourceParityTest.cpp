#include <gtest/gtest.h>
#include <type_traits>
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedEvent.h"
namespace { TEST(HandshakeEventSourceParityTest, SocketAccessorRemainsTyped) { using E=::jxx::ext::net::ssl::HandshakeCompletedEvent; EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&E::getSocket)>)); EXPECT_TRUE((std::is_member_function_pointer_v<decltype(&E::getSession)>)); } }
