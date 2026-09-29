#include "nio/channels/jxx.nio.channels.AlreadyBoundException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> AlreadyBoundException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* AlreadyBoundException::typeName() const noexcept {
    return "jxx.nio.channels.AlreadyBoundException";
}

} // namespace jxx::nio::channels
