#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.ClassInitialization.h"
#include <mutex>
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectionKey.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.NativeSelectorProvider.h"
namespace jxx::nio::channels::spi::internal {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
        (void)::jxx::lang::ClassBase<NativeSelectionKey, ::jxx::nio::channels::spi::AbstractSelectionKey>::Class();
        (void)::jxx::lang::ClassBase<NativeSelectorProvider, ::jxx::nio::channels::spi::SelectorProvider>::Class();
    });
}
}
