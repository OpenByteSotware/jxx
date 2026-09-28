#include <gtest/gtest.h>

#include <type_traits>

#include "ext/net/jxx.ext.net.SocketFactory.h"
#include "ext/net/ssl/jxx.ext.net.ssl.HandshakeCompletedListener.h"
#include "net/jxx.net.Socket.h"
#include "util/jxx.util.EventListener.h"

namespace {

TEST(ImportHierarchyParityTest, SocketFactoryCreatesUnconnectedSocket) {
    const auto factory =
        ::jxx::ext::net::SocketFactory::getDefault();
    ASSERT_NE(factory, nullptr);

    const auto socket = factory->createSocket();
    ASSERT_NE(socket, nullptr);
    EXPECT_FALSE(socket->isConnected());
}

TEST(ImportHierarchyParityTest, HandshakeListenerExtendsEventListener) {
    EXPECT_TRUE((std::is_base_of_v<
        ::jxx::util::EventListener,
        ::jxx::ext::net::ssl::HandshakeCompletedListener>));
}

} // namespace
