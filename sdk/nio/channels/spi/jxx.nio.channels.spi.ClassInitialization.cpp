#include "nio/channels/spi/jxx.nio.channels.spi.ClassInitialization.h"
#include <mutex>
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractInterruptibleChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectableChannel.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelector.h"
#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"
#include "nio/channels/spi/internal/jxx.nio.channels.spi.internal.ClassInitialization.h"
namespace jxx::nio::channels::spi {
void initializeClasses()
{
    static std::once_flag flag;
    std::call_once(flag, [] {
            ::jxx::lang::class_info_detail::registerClassInfo<AbstractInterruptibleChannel>();
            ::jxx::lang::class_info_detail::registerClassInfo<AbstractSelectableChannel>();
            ::jxx::lang::class_info_detail::registerClassInfo<AbstractSelectionKey>();
            ::jxx::lang::class_info_detail::registerClassInfo<AbstractSelector>();
            ::jxx::lang::class_info_detail::registerClassInfo<SelectorProvider>();
        (void)::jxx::lang::ClassBase<AbstractInterruptibleChannel, ::jxx::lang::Object, ::jxx::nio::channels::InterruptibleChannel>::Class();
        (void)::jxx::lang::ClassBase<AbstractSelectableChannel, ::jxx::nio::channels::SelectableChannel>::Class();
        (void)::jxx::lang::ClassBase<AbstractSelectionKey, ::jxx::nio::channels::SelectionKey>::Class();
        (void)::jxx::lang::ClassBase<AbstractSelector, ::jxx::nio::channels::Selector>::Class();
        (void)::jxx::lang::ClassBase<SelectorProvider, ::jxx::lang::Object>::Class();
    });
}

void initialize()
{
    initializeClasses();
    ::jxx::nio::channels::spi::internal::initialize();
}

}
