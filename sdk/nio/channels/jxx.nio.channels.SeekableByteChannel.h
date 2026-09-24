#pragma once

#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"

namespace jxx::nio::channels
{
    class SeekableByteChannel
        : public jxx::lang::InterfaceBase<
              SeekableByteChannel,
              ReadableByteChannel,
              WritableByteChannel>
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
