#pragma once

#include "nio/channels/jxx.nio.channels.Channel.h"

namespace jxx::nio::channels {

class InterruptibleChannel
    : public ::jxx::lang::InterfaceBase<
          InterruptibleChannel,
          Channel> {
public:
    ~InterruptibleChannel() override = default;
    virtual void close() override = 0;
};

} // namespace jxx::nio::channels
