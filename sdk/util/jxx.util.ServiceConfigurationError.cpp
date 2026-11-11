#include "util/jxx.util.ServiceConfigurationError.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Throwable.h"

namespace jxx::util {

::jxx::Ptr<::jxx::lang::ClassAny> ServiceConfigurationError::Class() {
    return JxxClassInfoMarker::Class();
}

ServiceConfigurationError::ServiceConfigurationError(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {}

ServiceConfigurationError::ServiceConfigurationError(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : JxxSuper(message, cause) {}

const char* ServiceConfigurationError::typeName() const noexcept {
    return "ServiceConfigurationError";
}

} // namespace jxx::util
