#pragma once

#include "nio/channels/jxx.nio.channels.ByteChannel.h"

namespace jxx::nio::channels
{
    class SeekableByteChannel
        : public jxx::lang::InterfaceBase<
              SeekableByteChannel,
              ByteChannel>
    {
    public:
        ~SeekableByteChannel() override = default;

    public:
        virtual jxx::lang::jlong position() const = 0;
        virtual jxx::Ptr<SeekableByteChannel> position(jxx::lang::jlong newPosition) = 0;
        virtual jxx::lang::jlong size() const = 0;
        virtual jxx::Ptr<SeekableByteChannel> truncate(jxx::lang::jlong size) = 0;
    };
}
