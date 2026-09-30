#include "nio/channels/jxx.nio.channels.SocketChannel.h"

namespace jxx::nio::channels {

SocketChannel::~SocketChannel()
{
    try {
        close();
    }
    catch (...) {
    }
}

} // namespace jxx::nio::channels
