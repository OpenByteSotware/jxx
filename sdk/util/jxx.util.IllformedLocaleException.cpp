#include "util/jxx.util.IllformedLocaleException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::util {

::jxx::Ptr<::jxx::lang::ClassAny> IllformedLocaleException::Class() {
    return JxxClassInfoMarker::Class();
}

IllformedLocaleException::IllformedLocaleException()
    : ::jxx::lang::RuntimeException()
    , errorIndex_(-1) {}

IllformedLocaleException::IllformedLocaleException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : ::jxx::lang::RuntimeException(message)
    , errorIndex_(-1) {}

IllformedLocaleException::IllformedLocaleException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    ::jxx::lang::jint errorIndex)
    : ::jxx::lang::RuntimeException(message)
    , errorIndex_(errorIndex) {}

::jxx::lang::jint IllformedLocaleException::getErrorIndex() const noexcept {
    return errorIndex_;
}

} // namespace jxx::util
