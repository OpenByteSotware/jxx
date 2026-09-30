#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectorProvider.h"

#include "nio/channels/jxx.nio.channels.Selector.h"
#include "nio/channels/jxx.nio.channels.ServerSocketChannel.h"
#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace jxx::nio::channels::spi::internal {

::jxx::Ptr<::jxx::nio::channels::SocketChannel>
NativeSelectorProvider::openSocketChannel() {
    return ::jxx::nio::channels::SocketChannel::open();
}

::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>
NativeSelectorProvider::openServerSocketChannel() {
    return ::jxx::nio::channels::ServerSocketChannel::open();
}

::jxx::Ptr<::jxx::nio::channels::Selector>
NativeSelectorProvider::openSelector() {
    return ::jxx::nio::channels::Selector::open();
}

} // namespace jxx::nio::channels::spi::internal
