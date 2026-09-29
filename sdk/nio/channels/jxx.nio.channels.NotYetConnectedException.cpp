#include "nio/channels/jxx.nio.channels.NotYetConnectedException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> NotYetConnectedException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* NotYetConnectedException::typeName() const noexcept {
    return "jxx.nio.channels.NotYetConnectedException";
}

} // namespace jxx::nio::channels
