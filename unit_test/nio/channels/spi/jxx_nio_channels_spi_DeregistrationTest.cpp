#include <gtest/gtest.h>

#include "nio/channels/jxx.nio.channels.ClosedSelectorException.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace {

TEST(
    SelectorDeregistrationParity,
    CancelledKeyIsRemovedFromChannelDuringSelection) {

    const auto selector =
        ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();

    channel->configureBlocking(false);
    const auto key = channel->register_(
        selector,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);

    ASSERT_TRUE(channel->isRegistered());

    key->cancel();
    EXPECT_FALSE(key->isValid());

    (void)selector->selectNow();

    EXPECT_FALSE(channel->isRegistered());
    EXPECT_EQ(nullptr, channel->keyFor(selector));
    EXPECT_NO_THROW((void)channel->configureBlocking(true));

    channel->close();
    selector->close();
}

TEST(
    SelectorDeregistrationParity,
    OneCancellationPreservesOtherSelectorRegistration) {

    const auto selectorA =
        ::jxx::nio::channels::Selector::open();
    const auto selectorB =
        ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();

    channel->configureBlocking(false);

    const auto keyA = channel->register_(
        selectorA,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);
    const auto keyB = channel->register_(
        selectorB,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);

    keyA->cancel();
    (void)selectorA->selectNow();

    EXPECT_TRUE(channel->isRegistered());
    EXPECT_EQ(nullptr, channel->keyFor(selectorA));
    EXPECT_EQ(keyB, channel->keyFor(selectorB));

    keyB->cancel();
    (void)selectorB->selectNow();

    EXPECT_FALSE(channel->isRegistered());

    channel->close();
    selectorA->close();
    selectorB->close();
}

TEST(
    SelectorDeregistrationParity,
    SelectorCloseDeregistersChannelImmediately) {

    const auto selector =
        ::jxx::nio::channels::Selector::open();
    const auto channel =
        ::jxx::nio::channels::SocketChannel::open();

    channel->configureBlocking(false);
    const auto key = channel->register_(
        selector,
        ::jxx::nio::channels::SelectionKey::OP_CONNECT_);

    selector->close();

    EXPECT_FALSE(key->isValid());
    EXPECT_FALSE(channel->isRegistered());
    EXPECT_EQ(nullptr, channel->keyFor(selector));
    EXPECT_NO_THROW((void)channel->configureBlocking(true));

    channel->close();
}

TEST(
    SelectorDeregistrationParity,
    ClosedSelectorRejectsKeySetsAndSelection) {

    const auto selector =
        ::jxx::nio::channels::Selector::open();
    selector->close();

    EXPECT_THROW(
        (void)selector->keys(),
        ::jxx::nio::channels::ClosedSelectorException);

    EXPECT_THROW(
        (void)selector->selectedKeys(),
        ::jxx::nio::channels::ClosedSelectorException);

    EXPECT_THROW(
        (void)selector->selectNow(),
        ::jxx::nio::channels::ClosedSelectorException);
}

} // namespace
