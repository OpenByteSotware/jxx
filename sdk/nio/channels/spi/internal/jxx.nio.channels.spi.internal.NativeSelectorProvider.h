#pragma once

#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"

namespace jxx::nio::channels::spi::internal {

class NativeSelectorProvider final
    : public ::jxx::lang::ClassBase<
          NativeSelectorProvider,
          ::jxx::nio::channels::spi::SelectorProvider> {
public:
    using JxxSuper = ::jxx::nio::channels::spi::SelectorProvider;

    NativeSelectorProvider() = default;
    ~NativeSelectorProvider() override = default;

    ::jxx::Ptr<::jxx::nio::channels::SocketChannel>
    openSocketChannel() override;
    ::jxx::Ptr<::jxx::nio::channels::ServerSocketChannel>
    openServerSocketChannel() override;
    ::jxx::Ptr<::jxx::nio::channels::Selector>
    openSelector() override;
};

} // namespace jxx::nio::channels::spi::internal
