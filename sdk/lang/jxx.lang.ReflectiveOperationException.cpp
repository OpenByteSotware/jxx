#include "lang/jxx.lang.ReflectiveOperationException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> ReflectiveOperationException::Class() {
    return JxxClassInfoMarker::Class();
}

ReflectiveOperationException::ReflectiveOperationException()
    : JxxSuper() {
}

ReflectiveOperationException::ReflectiveOperationException(
    const ::jxx::Ptr<String>& message)
    : JxxSuper(message) {
}

ReflectiveOperationException::ReflectiveOperationException(
    const char* message)
    : JxxSuper(message) {
}

ReflectiveOperationException::ReflectiveOperationException(
    const ::jxx::Ptr<String>& message,
    const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(message, cause) {
}

ReflectiveOperationException::ReflectiveOperationException(
    const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(cause) {
}

const char* ReflectiveOperationException::typeName() const noexcept {
    return "ReflectiveOperationException";
}

} // namespace jxx::lang
