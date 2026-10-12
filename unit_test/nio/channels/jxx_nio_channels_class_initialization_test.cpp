#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"
#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"
#include "nio/channels/jxx.nio.channels.ByteChannel.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "nio/channels/jxx.nio.channels.SelectableChannel.h"
#include "nio/channels/jxx.nio.channels.SelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

TEST(JxxNioChannelsClassInitialization, PublicTypesHaveClassMetadata)
{
    ::jxx::initialize();

    EXPECT_NE(::jxx::nio::channels::AlreadyBoundException::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::AsynchronousCloseException::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::ByteChannel::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::InterruptibleChannel::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::NetworkChannel::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::SelectableChannel::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::SelectionKey::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::Selector::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::ServerSocketChannel::Class(), nullptr);
    EXPECT_NE(::jxx::nio::channels::SocketChannel::Class(), nullptr);
}
