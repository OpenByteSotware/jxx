#include <gtest/gtest.h>
#include <type_traits>

#include "net/jxx.net.StandardSocketOptions.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace {

TEST(SocketChannelCombinedParityTest, BindMatchesNetworkChannel) {
    using Method = ::jxx::Ptr<::jxx::nio::channels::NetworkChannel> (
        ::jxx::nio::channels::SocketChannel::*)(
            const ::jxx::Ptr<::jxx::net::SocketAddress>&);
    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::bind)),
        Method>));
}

TEST(SocketChannelCombinedParityTest, SupportedOptionsUsesElementType) {
    using Option = ::jxx::nio::channels::NetworkChannel::Option;
    using Method = ::jxx::Ptr<::jxx::util::Set<Option>> (
        ::jxx::nio::channels::SocketChannel::*)() const;
    EXPECT_TRUE((std::is_same_v<
        decltype(static_cast<Method>(
            &::jxx::nio::channels::SocketChannel::supportedOptions)),
        Method>));
}

TEST(SocketChannelCombinedParityTest, StandardFieldsUseUnderscores) {
    EXPECT_NE(::jxx::net::StandardSocketOptions::SO_KEEPALIVE_, nullptr);
    EXPECT_NE(::jxx::net::StandardSocketOptions::SO_REUSEADDR_, nullptr);
    EXPECT_NE(::jxx::net::StandardSocketOptions::SO_SNDBUF_, nullptr);
    EXPECT_NE(::jxx::net::StandardSocketOptions::SO_RCVBUF_, nullptr);
    EXPECT_NE(::jxx::net::StandardSocketOptions::TCP_NODELAY_, nullptr);
}

} // namespace
