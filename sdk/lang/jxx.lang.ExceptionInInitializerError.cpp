#include "lang/jxx.lang.ExceptionInInitializerError.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> ExceptionInInitializerError::Class() {
    return JxxClassInfoMarker::Class();
}

ExceptionInInitializerError::ExceptionInInitializerError()
    : JxxSuper() {
}

ExceptionInInitializerError::ExceptionInInitializerError(
    const ::jxx::Ptr<Throwable>& exception)
    : JxxSuper()
    , exception_(exception) {
    initCause(exception);
}

ExceptionInInitializerError::ExceptionInInitializerError(
    const ::jxx::Ptr<String>& message)
    : JxxSuper(message) {
}

ExceptionInInitializerError::ExceptionInInitializerError(
    const char* message)
    : JxxSuper(message) {
}

::jxx::Ptr<Throwable>
ExceptionInInitializerError::getException() const {
    return exception_;
}

::jxx::Ptr<Throwable>
ExceptionInInitializerError::getCause() const {
    return exception_;
}

const char* ExceptionInInitializerError::typeName() const noexcept {
    return "ExceptionInInitializerError";
}

} // namespace jxx::lang
