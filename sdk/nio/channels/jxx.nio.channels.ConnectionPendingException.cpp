#include "nio/channels/jxx.nio.channels.ConnectionPendingException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> ConnectionPendingException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* ConnectionPendingException::typeName() const noexcept {
    return "jxx.nio.channels.ConnectionPendingException";
}

} // namespace jxx::nio::channels
