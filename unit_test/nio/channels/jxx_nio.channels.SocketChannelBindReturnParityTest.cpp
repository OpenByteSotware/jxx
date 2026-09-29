#include <gtest/gtest.h>
#include <type_traits>

#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace {

TEST(SocketChannelBindReturnParityTest, MatchesNetworkChannelOverride) {
    using Method = ::jxx::Ptr<::jxx::nio::channels::NetworkChannel> (
        ::jxx::nio::channels::SocketChannel::*)(
            const ::jxx::Ptr<::jxx::net::SocketAddress>&);

    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::bind)),
        Method>));
}

} // namespace
