#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectorProvider.h"

#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelector.h"
#include "net/internal/jxx.net.internal.NativeSocketState.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace jxx::nio::channels::spi::internal {

::jxx::Ptr<::jxx::nio::channels::SocketChannel>
NativeSelectorProvider::openSocketChannel() {
    return ::jxx::NEW<::jxx::nio::channels::SocketChannel>();
}

::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>
NativeSelectorProvider::openServerSocketChannel() {
    auto channel = ::jxx::NEW<::jxx::nio::channels::ServerSocketChannel>();
    channel->state_->serverChannel = channel;
    return channel;
}

::jxx::Ptr<::jxx::nio::channels::Selector>
NativeSelectorProvider::openSelector() {
    return ::jxx::CAST<::jxx::nio::channels::Selector>(::jxx::NEW<::jxx::nio::channels::spi::internal::NativeSelector>(::jxx::CAST<::jxx::nio::channels::spi::SelectorProvider>(thisPtr())));
}

} // namespace jxx::nio::channels::spi::internal
