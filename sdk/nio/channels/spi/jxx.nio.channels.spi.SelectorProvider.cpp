#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"

#include <mutex>

#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectorProvider.h"

namespace jxx::nio::channels::spi {

::jxx::Ptr<::jxx::nio::channels::Channel>
SelectorProvider::inheritedChannel() {
    return nullptr;
}

::jxx::Ptr<SelectorProvider> SelectorProvider::provider() {
    static std::once_flag flag;
    static ::jxx::Ptr<SelectorProvider> instance;
    std::call_once(flag, [] {
        instance = ::jxx::CAST<SelectorProvider>(
            ::jxx::NEW<internal::NativeSelectorProvider>());
    });
    return instance;
}

} // namespace jxx::nio::channels::spi
