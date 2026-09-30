#pragma once

#include "nio/channels/jxx.nio.channels.AbstractInterruptibleChannel.h"

namespace jxx::nio::channels {
class SelectionKey;
class Selector;
namespace spi { class SelectorProvider; }

class SelectableChannel
    : public AbstractInterruptibleChannel {
public:
    using JxxSuper = AbstractInterruptibleChannel;

    ~SelectableChannel() override = default;

    virtual ::jxx::Ptr<spi::SelectorProvider> provider() const = 0;
    virtual ::jxx::lang::jint validOps() const noexcept = 0;
    virtual ::jxx::lang::jbool isRegistered() const = 0;
    virtual ::jxx::Ptr<SelectionKey> keyFor(
        const ::jxx::Ptr<Selector>& selector) const = 0;
    virtual ::jxx::Ptr<SelectionKey> register_(
        const ::jxx::Ptr<Selector>& selector,
        ::jxx::lang::jint operations) = 0;
    virtual ::jxx::Ptr<SelectionKey> register_(
        const ::jxx::Ptr<Selector>& selector,
        ::jxx::lang::jint operations,
        const ::jxx::Ptr<::jxx::lang::Object>& attachment) = 0;
    virtual ::jxx::lang::jbool isBlocking() const noexcept = 0;
    virtual ::jxx::Ptr<::jxx::lang::Object> blockingLock() = 0;

protected:
    SelectableChannel() = default;
};

} // namespace jxx::nio::channels
