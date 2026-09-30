#include <gtest/gtest.h>
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "net/jxx.net.ServerSocket.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
namespace {
TEST(SelectorProviderParity, StaticOpenUsesDefaultProvider){auto provider=::jxx::nio::channels::spi::SelectorProvider::provider();ASSERT_NE(nullptr,provider);auto socket=::jxx::nio::channels::SocketChannel::open();auto server=::jxx::nio::channels::ServerSocketChannel::open();auto selector=::jxx::nio::channels::Selector::open();ASSERT_NE(nullptr,socket);ASSERT_NE(nullptr,server);ASSERT_NE(nullptr,selector);EXPECT_EQ(provider, socket->provider());EXPECT_EQ(provider, server->provider());EXPECT_EQ(provider, selector->provider());socket->close();server->close();selector->close();}
TEST(SelectorProviderParity, ServerSocketViewReferencesProviderChannel){auto server=::jxx::nio::channels::ServerSocketChannel::open();ASSERT_NE(nullptr,server->socket());EXPECT_EQ(server,server->socket()->getChannel());server->close();}
TEST(SelectorProviderParity, InheritedChannelDefaultsToNull){auto provider=::jxx::nio::channels::spi::SelectorProvider::provider();EXPECT_EQ(nullptr,provider->inheritedChannel());}
}
