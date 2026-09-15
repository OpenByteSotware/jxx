#include "lang/jxx.lang.BootstrapMethodError.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {
::jxx::Ptr<ClassAny> BootstrapMethodError::Class() { return JxxClassInfoMarker::Class(); }
BootstrapMethodError::BootstrapMethodError() : JxxSuper() {}
BootstrapMethodError::BootstrapMethodError(const ::jxx::Ptr<String>& message) : JxxSuper(message) {}
BootstrapMethodError::BootstrapMethodError(const char* message) : JxxSuper(message) {}
BootstrapMethodError::BootstrapMethodError(const ::jxx::Ptr<Throwable>& cause) : JxxSuper(cause) {}
BootstrapMethodError::BootstrapMethodError(const ::jxx::Ptr<String>& message, const ::jxx::Ptr<Throwable>& cause) : JxxSuper(message, cause) {}
const char* BootstrapMethodError::typeName() const noexcept { return "BootstrapMethodError"; }
} // namespace jxx::lang
