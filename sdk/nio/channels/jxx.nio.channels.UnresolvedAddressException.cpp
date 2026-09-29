#include "nio/channels/jxx.nio.channels.UnresolvedAddressException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> UnresolvedAddressException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* UnresolvedAddressException::typeName() const noexcept {
    return "jxx.nio.channels.UnresolvedAddressException";
}

} // namespace jxx::nio::channels
