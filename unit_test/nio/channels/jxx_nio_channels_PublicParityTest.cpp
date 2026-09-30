#include <gtest/gtest.h>

#include "lang/jxx.lang.Integer.h"
#include "net/jxx.net.StandardSocketOptions.h"
#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"
#include "nio/channels/jxx.nio.channels.NotYetBoundException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace {

TEST(NioPublicParity, UnboundServerAcceptThrowsNotYetBound) {
    const auto channel =
        ::jxx::nio::channels::ServerSocketChannel::open();
    EXPECT_THROW(
        (void)channel->accept(),
        ::jxx::nio::channels::NotYetBoundException);
    channel->close();
}

TEST(NioPublicParity, BlockingRegistrationThrowsIllegalBlockingMode) {
    const auto selector = ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();
    EXPECT_THROW(
        (void)channel->register_(
            selector,
            ::jxx::nio::channels::SelectionKey::OP_CONNECT_),
        ::jxx::nio::channels::IllegalBlockingModeException);
    channel->close();
    selector->close();
}

TEST(NioPublicParity, RegisterOverloadsUseParityName) {
    const auto selector = ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();
    channel->configureBlocking(false);

    const auto key = channel->register_(
        selector,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);

    ASSERT_NE(nullptr, key);
    EXPECT_EQ(channel, channel->keyFor(selector)->channel());

    key->cancel();
    channel->close();
    selector->close();
}

TEST(NioPublicParity, SocketChannelSupportsSoLinger) {
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();

    EXPECT_TRUE(channel->supportedOptions()->contains(
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::net::StandardSocketOptions::SO_LINGER_)));

    channel->setOption(
        ::jxx::net::StandardSocketOptions::SO_LINGER_,
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Integer::valueOf(3)));

    const auto enabled = ::jxx::CAST<::jxx::lang::Integer>(
        channel->getOption(
            ::jxx::net::StandardSocketOptions::SO_LINGER_));
    ASSERT_NE(nullptr, enabled);
    EXPECT_EQ(3, enabled->intValue());

    channel->setOption(
        ::jxx::net::StandardSocketOptions::SO_LINGER_,
        ::jxx::CAST<::jxx::lang::Object>(
            ::jxx::lang::Integer::valueOf(-1)));

    const auto disabled = ::jxx::CAST<::jxx::lang::Integer>(
        channel->getOption(
            ::jxx::net::StandardSocketOptions::SO_LINGER_));
    ASSERT_NE(nullptr, disabled);
    EXPECT_EQ(-1, disabled->intValue());

    channel->close();
}

} // namespace
