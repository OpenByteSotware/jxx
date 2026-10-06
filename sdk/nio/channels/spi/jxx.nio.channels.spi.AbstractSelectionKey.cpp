#include "nio/channels/spi/jxx.nio.channels.spi.AbstractSelectionKey.h"
#include "nio/channels/jxx.nio.channels.Selector.h"
namespace jxx::nio::channels::spi {
::jxx::Ptr<::jxx::lang::ClassAny> AbstractSelectionKey::Class(){return JxxClassInfoMarker::Class();}
::jxx::lang::jbool AbstractSelectionKey::isValid()const noexcept{return valid_.load(std::memory_order_acquire);}
void AbstractSelectionKey::cancelWithReference_(
    const ::jxx::Ptr<::jxx::nio::channels::SelectionKey>& key) {
    ::jxx::lang::jbool expected = true;
    if (!valid_.compare_exchange_strong(
            expected,
            false,
            std::memory_order_acq_rel)) {
        return;
    }

    const auto owner = selector();
    if (owner != nullptr && key != nullptr) {
        owner->cancelKey(key);
    }
}

void AbstractSelectionKey::cancel() {
    cancelWithReference_(
        ::jxx::CAST<::jxx::nio::channels::SelectionKey>(thisPtr()));
}

void AbstractSelectionKey::invalidate_()noexcept{valid_.store(false,std::memory_order_release);}
}
