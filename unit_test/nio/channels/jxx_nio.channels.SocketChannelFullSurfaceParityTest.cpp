#include <gtest/gtest.h>
#include <type_traits>
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace { TEST(SocketChannelFullSurfaceParityTest, ImplementsJava8ChannelContracts){using C=::jxx::nio::channels::SocketChannel;EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::ByteChannel,C>));EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::ScatteringByteChannel,C>));EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::GatheringByteChannel,C>));EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::NetworkChannel,C>));} }
