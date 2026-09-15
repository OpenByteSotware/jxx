#include "lang/jxx.lang.ClassNotFoundException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> ClassNotFoundException::Class() {
    return JxxClassInfoMarker::Class();
}

ClassNotFoundException::ClassNotFoundException()
    : JxxSuper() {
}

ClassNotFoundException::ClassNotFoundException(
    const ::jxx::Ptr<String>& message)
    : JxxSuper(message) {
}

ClassNotFoundException::ClassNotFoundException(const char* message)
    : JxxSuper(message) {
}

ClassNotFoundException::ClassNotFoundException(
    const ::jxx::Ptr<String>& message,
    const ::jxx::Ptr<Throwable>& exception)
    : JxxSuper(message, exception)
    , exception_(exception) {
}

::jxx::Ptr<Throwable> ClassNotFoundException::getException() const {
    return exception_;
}

::jxx::Ptr<Throwable> ClassNotFoundException::getCause() const {
    return exception_;
}

const char* ClassNotFoundException::typeName() const noexcept {
    return "ClassNotFoundException";
}

} // namespace jxx::lang
