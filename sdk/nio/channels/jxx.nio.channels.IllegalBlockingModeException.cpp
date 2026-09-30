#include "nio/channels/jxx.nio.channels.IllegalBlockingModeException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny>
IllegalBlockingModeException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* IllegalBlockingModeException::typeName() const noexcept {
    return "jxx.nio.channels.IllegalBlockingModeException";
}

} // namespace jxx::nio::channels
