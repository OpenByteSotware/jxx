#pragma once

#include "jxx_types.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/channels/jxx.nio.channels.Channel.h"

namespace jxx::nio::channels
{
    class ReadableByteChannel : public Channel
    {
    public:
        ~ReadableByteChannel() override = default;

    public:
        virtual jxx::lang::jint read(jxx::Ptr<jxx::nio::ByteBuffer> dst) = 0;
    };
}
