#include "nio/channels/jxx.nio.channels.NoConnectionPendingException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> NoConnectionPendingException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* NoConnectionPendingException::typeName() const noexcept {
    return "jxx.nio.channels.NoConnectionPendingException";
}

} // namespace jxx::nio::channels
