#include <gtest/gtest.h>

#include "nio/channels/jxx.nio.channels.AbstractInterruptibleChannel.h"
#include "nio/channels/jxx.nio.channels.AbstractSelectableChannel.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"

namespace {

TEST(SelectableHierarchyParity, SocketChannelExposesSelectableHierarchy) {
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();

    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(channel));
    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::nio::channels::InterruptibleChannel>(channel));
    EXPECT_NE(nullptr, channel->provider());
    EXPECT_NE(nullptr, channel->blockingLock());
    EXPECT_FALSE(channel->isRegistered());

    channel->close();
}

TEST(SelectableHierarchyParity, ServerChannelExposesSelectableHierarchy) {
    const auto channel =
        ::jxx::nio::channels::ServerSocketChannel::open();

    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(channel));
    EXPECT_NE(nullptr,
        ::jxx::CAST<::jxx::nio::channels::InterruptibleChannel>(channel));
    EXPECT_EQ(
        ::jxx::nio::channels::SelectionKey::OP_ACCEPT_,
        channel->validOps());
    EXPECT_NE(nullptr, channel->provider());

    channel->close();
}

TEST(SelectableHierarchyParity, RegisterMarksChannelRegistered) {
    const auto selector = ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();
    channel->configureBlocking(false);

    const auto key = channel->register_(
        selector,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);

    ASSERT_NE(nullptr, key);
    EXPECT_TRUE(channel->isRegistered());
    EXPECT_EQ(
        ::jxx::CAST<::jxx::nio::channels::SelectableChannel>(channel),
        key->channel());

    key->cancel();
    channel->close();
    selector->close();
}

TEST(SelectableHierarchyParity, ProviderCreatesPublicChannelTypes) {
    const auto provider =
        ::jxx::nio::channels::spi::SelectorProvider::provider();

    ASSERT_NE(nullptr, provider);
    const auto socketChannel = provider->openSocketChannel();
    const auto serverChannel = provider->openServerSocketChannel();
    const auto selector = provider->openSelector();

    EXPECT_NE(nullptr, socketChannel);
    EXPECT_NE(nullptr, serverChannel);
    EXPECT_NE(nullptr, selector);

    socketChannel->close();
    serverChannel->close();
    selector->close();
}

} // namespace
