#pragma once

#include <mutex>

#include "nio/channels/jxx.nio.channels.SelectableChannel.h"

namespace jxx::nio::channels {

class AbstractSelectableChannel
    : public SelectableChannel {
public:
    using JxxSuper = SelectableChannel;

    ~AbstractSelectableChannel() override = default;

    ::jxx::Ptr<spi::SelectorProvider> provider() const override;
    ::jxx::lang::jbool isRegistered() const override;
    ::jxx::Ptr<::jxx::lang::Object> blockingLock() override;

protected:
    explicit AbstractSelectableChannel(
        const ::jxx::Ptr<spi::SelectorProvider>& provider = nullptr);
    void setRegistered_(::jxx::lang::jbool registered) noexcept;

private:
    ::jxx::Ptr<spi::SelectorProvider> provider_;
    ::jxx::Ptr<::jxx::lang::Object> blockingLock_;
    mutable std::mutex registrationMutex_;
    ::jxx::lang::jbool registered_ = false;
};

} // namespace jxx::nio::channels
