#include "nio/channels/jxx.nio.channels.AlreadyConnectedException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> AlreadyConnectedException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* AlreadyConnectedException::typeName() const noexcept {
    return "jxx.nio.channels.AlreadyConnectedException";
}

} // namespace jxx::nio::channels
