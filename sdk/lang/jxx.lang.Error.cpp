#include "lang/jxx.lang.Error.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> Error::Class() {
    return JxxClassInfoMarker::Class();
}

Error::Error()
    : JxxSuper() {
}

Error::Error(const ::jxx::Ptr<String>& message)
    : JxxSuper(message) {
}

Error::Error(const char* message)
    : JxxSuper(message) {
}

Error::Error(
    const ::jxx::Ptr<String>& message,
    const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(message, cause) {
}

Error::Error(const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(cause) {
}

Error::Error(
    const ::jxx::Ptr<String>& message,
    const ::jxx::Ptr<Throwable>& cause,
    jbool enableSuppression,
    jbool writableStackTrace)
    : JxxSuper(
          message,
          cause,
          enableSuppression,
          writableStackTrace) {
}

const char* Error::typeName() const noexcept {
    return "Error";
}

} // namespace jxx::lang
