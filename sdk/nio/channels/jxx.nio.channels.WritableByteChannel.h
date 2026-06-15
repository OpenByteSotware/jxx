#pragma once

#include "lang/jxx_types.h"
#include "nio/jxx.nio.ByteBuffer.h"
#include "nio/channels/jxx.nio.channels.Channel.h"

namespace jxx::nio::channels
{
    class WritableByteChannel : public Channel
    {
    public:
        ~WritableByteChannel() override = default;

    public:
        virtual jxx::lang::jint write(jxx::Ptr<jxx::nio::ByteBuffer> src) = 0;
    };
}
