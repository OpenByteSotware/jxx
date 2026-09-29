#include <gtest/gtest.h>
#include <type_traits>
#include "nio/channels/jxx.nio.channels.SocketChannel.h"
namespace {
TEST(SocketChannelAddressParityTest, AddressGettersReturnSocketAddress) {
    using Method = ::jxx::Ptr<::jxx::net::SocketAddress> (
        ::jxx::nio::channels::SocketChannel::*)() const;
    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::getLocalAddress)), Method>));
    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::getRemoteAddress)), Method>));
}
}
