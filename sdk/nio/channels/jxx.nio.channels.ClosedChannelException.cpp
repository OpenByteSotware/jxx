#include "nio/channels/jxx.nio.channels.ClosedChannelException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> ClosedChannelException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* ClosedChannelException::typeName() const noexcept {
    return "jxx.nio.channels.ClosedChannelException";
}

} // namespace jxx::nio::channels
