#pragma once

#include <atomic>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "nio/channels/jxx.nio.channels.InterruptibleChannel.h"

namespace jxx::nio::channels {

class AbstractInterruptibleChannel
    : public ::jxx::lang::ClassBase<
          AbstractInterruptibleChannel,
          ::jxx::lang::Object,
          InterruptibleChannel> {
public:
    using JxxSuper = ::jxx::lang::Object;
    using Super = ::jxx::lang::ClassBase<
        AbstractInterruptibleChannel,
        JxxSuper,
        InterruptibleChannel>;

    ~AbstractInterruptibleChannel() override = default;

    ::jxx::lang::jbool isOpen() const override;
    void close() override;

protected:
    AbstractInterruptibleChannel() = default;
    virtual void implCloseChannel() = 0;
    void markClosed() noexcept;

private:
    std::atomic<::jxx::lang::jbool> open_{true};
};

} // namespace jxx::nio::channels
