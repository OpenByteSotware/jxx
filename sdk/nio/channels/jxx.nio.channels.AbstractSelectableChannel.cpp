#include "nio/channels/jxx.nio.channels.AbstractSelectableChannel.h"

#include "nio/channels/spi/jxx.nio.channels.spi.SelectorProvider.h"

namespace jxx::nio::channels {

AbstractSelectableChannel::AbstractSelectableChannel(
    const ::jxx::Ptr<spi::SelectorProvider>& provider)
    : provider_(provider),
      blockingLock_(::jxx::NEW<::jxx::lang::Object>()) {
}

::jxx::Ptr<spi::SelectorProvider>
AbstractSelectableChannel::provider() const {
    return provider_ == nullptr
        ? spi::SelectorProvider::provider()
        : provider_;
}

::jxx::lang::jbool AbstractSelectableChannel::isRegistered() const {
    std::lock_guard<std::mutex> lock(registrationMutex_);
    return registered_;
}

::jxx::Ptr<::jxx::lang::Object>
AbstractSelectableChannel::blockingLock() {
    return blockingLock_;
}

void AbstractSelectableChannel::setRegistered_(
    ::jxx::lang::jbool registered) noexcept {
    try {
        std::lock_guard<std::mutex> lock(registrationMutex_);
        registered_ = registered;
    }
    catch (...) {
    }
}

} // namespace jxx::nio::channels
