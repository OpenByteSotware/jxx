#include <gtest/gtest.h>
#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
TEST(SocketChannelSocketViewParityTest, ReturnsStableAssociatedSocket) {
    const auto channel = ::jxx::nio::channels::SocketChannel::open();
    const auto first = channel->socket();
    const auto second = channel->socket();
    ASSERT_NE(first, nullptr);
    EXPECT_EQ(first, second);
    EXPECT_EQ(first->getChannel(), channel);
    EXPECT_FALSE(first->isConnected());
}
TEST(SocketChannelSocketViewParityTest, ChannelCloseClosesSocketView) {
    const auto channel = ::jxx::nio::channels::SocketChannel::open();
    const auto socket = channel->socket();
    channel->close();
    EXPECT_FALSE(channel->isOpen());
    EXPECT_TRUE(socket->isClosed());
}
TEST(SocketChannelSocketViewParityTest, SocketCloseClosesChannel) {
    const auto channel = ::jxx::nio::channels::SocketChannel::open();
    const auto socket = channel->socket();
    socket->close();
    EXPECT_TRUE(socket->isClosed());
    EXPECT_FALSE(channel->isOpen());
    const auto buffer = ::jxx::nio::ByteBuffer::allocate(1);
    EXPECT_THROW(channel->read(buffer),
        ::jxx::nio::channels::ClosedChannelException);
}
} // namespace
