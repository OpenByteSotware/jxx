#include <gtest/gtest.h>
#include <type_traits>
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
TEST(SocketChannelFinishConnectParityTest, FinishConnectReturnsBoolean) {
    using Method = ::jxx::lang::jbool (
        ::jxx::nio::channels::SocketChannel::*)();
    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::finishConnect)),
        Method>));
}
}
