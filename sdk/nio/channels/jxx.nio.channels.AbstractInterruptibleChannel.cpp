#include "nio/channels/jxx.nio.channels.AbstractInterruptibleChannel.h"

namespace jxx::nio::channels {

::jxx::lang::jbool AbstractInterruptibleChannel::isOpen() const {
    return open_.load(std::memory_order_acquire);
}

void AbstractInterruptibleChannel::close() {
    ::jxx::lang::jbool expected = true;
    if (!open_.compare_exchange_strong(
            expected,
            false,
            std::memory_order_acq_rel)) {
        return;
    }
    implCloseChannel();
}

void AbstractInterruptibleChannel::markClosed() noexcept {
    open_.store(false, std::memory_order_release);
}

} // namespace jxx::nio::channels
