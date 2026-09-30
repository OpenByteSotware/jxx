#include "nio/channels/jxx.nio.channels.ClosedByInterruptException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> ClosedByInterruptException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* ClosedByInterruptException::typeName() const noexcept {
    return "jxx.nio.channels.ClosedByInterruptException";
}

} // namespace jxx::nio::channels
