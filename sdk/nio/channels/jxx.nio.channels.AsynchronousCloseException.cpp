#include "nio/channels/jxx.nio.channels.AsynchronousCloseException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> AsynchronousCloseException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* AsynchronousCloseException::typeName() const noexcept {
    return "jxx.nio.channels.AsynchronousCloseException";
}

} // namespace jxx::nio::channels
