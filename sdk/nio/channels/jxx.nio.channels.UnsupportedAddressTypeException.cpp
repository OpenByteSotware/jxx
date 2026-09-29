#include "nio/channels/jxx.nio.channels.UnsupportedAddressTypeException.h"

namespace jxx::nio::channels {

::jxx::Ptr<::jxx::lang::ClassAny> UnsupportedAddressTypeException::Class() {
    return JxxClassInfoMarker::Class();
}

const char* UnsupportedAddressTypeException::typeName() const noexcept {
    return "jxx.nio.channels.UnsupportedAddressTypeException";
}

} // namespace jxx::nio::channels
