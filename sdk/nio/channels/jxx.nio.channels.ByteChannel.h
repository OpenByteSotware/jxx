#pragma once
#include "nio/channels/jxx.nio.channels.ReadableByteChannel.h"
#include "nio/channels/jxx.nio.channels.WritableByteChannel.h"
namespace jxx::nio::channels {
class ByteChannel : public ::jxx::lang::InterfaceBase<ByteChannel,ReadableByteChannel,WritableByteChannel>{public:~ByteChannel() override=default;};
}
