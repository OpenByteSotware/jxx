#include <gtest/gtest.h>
#include <type_traits>
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
TEST(SocketChannelParityTest, ImplementsReadableAndWritableChannels) {
 EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::ReadableByteChannel,::jxx::nio::channels::SocketChannel>));
 EXPECT_TRUE((std::is_base_of_v<::jxx::nio::channels::WritableByteChannel,::jxx::nio::channels::SocketChannel>));
}
}
