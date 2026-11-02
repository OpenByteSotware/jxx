#include <gtest/gtest.h>
#include <type_traits>

#include "io/jxx.io.Closeable.h"
#include "lang/jxx.lang.AutoCloseable.h"
#include "nio/channels/jxx.nio.channels.ByteChannel.h"
#include "nio/channels/jxx.nio.channels.Channel.h"
#include "nio/channels/jxx.nio.channels.ClassInitialization.h"
#include "nio/channels/jxx.nio.channels.GatheringByteChannel.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"
#include "nio/channels/jxx.nio.channels.NetworkChannel.h"
#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
#include "nio/channels/jxx.nio.channels.ScatteringByteChannel.h"
#include "nio/channels/jxx.nio.channels.SeekableByteChannel.h"
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"

namespace channels = ::jxx::nio::channels;

TEST(NioChannelsInheritanceParity, ChannelDirectlyExtendsCloseable) {
    static_assert(std::is_base_of_v<::jxx::io::Closeable, channels::Channel>);
    static_assert(std::is_base_of_v<::jxx::lang::AutoCloseable, channels::Channel>);
    SUCCEED();
}

TEST(NioChannelsInheritanceParity, CompoundInterfacesMatchSourceHierarchy) {
    static_assert(std::is_base_of_v<channels::ReadableByteChannel, channels::ByteChannel>);
    static_assert(std::is_base_of_v<channels::WritableByteChannel, channels::ByteChannel>);
    static_assert(std::is_base_of_v<channels::ByteChannel, channels::SeekableByteChannel>);
    static_assert(std::is_base_of_v<channels::ReadableByteChannel, channels::ScatteringByteChannel>);
    static_assert(std::is_base_of_v<channels::WritableByteChannel, channels::GatheringByteChannel>);
    static_assert(std::is_base_of_v<channels::Channel, channels::InterruptibleChannel>);
    static_assert(std::is_base_of_v<channels::Channel, channels::NetworkChannel>);
    SUCCEED();
}

TEST(NioChannelsInitializationParity, RegistersPublicInterfaceHierarchy) {
    channels::initialize();
    EXPECT_NE(nullptr, channels::Channel::Class());
    EXPECT_NE(nullptr, channels::ReadableByteChannel::Class());
    EXPECT_NE(nullptr, channels::WritableByteChannel::Class());
    EXPECT_NE(nullptr, channels::ByteChannel::Class());
    EXPECT_NE(nullptr, channels::SeekableByteChannel::Class());
    EXPECT_NE(nullptr, channels::InterruptibleChannel::Class());
    EXPECT_NE(nullptr, channels::NetworkChannel::Class());
}
