#include "lang/jxx.lang.AssertionError.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {

::jxx::Ptr<Throwable> throwableCause(
    const ::jxx::Ptr<Object>& value) {
    return ::jxx::CAST<Throwable>(value);
}

} // namespace

::jxx::Ptr<ClassAny> AssertionError::Class() {
    return JxxClassInfoMarker::Class();
}

AssertionError::AssertionError()
    : JxxSuper() {
}

AssertionError::AssertionError(jbool value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(jchar value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(jdouble value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(jfloat value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(jint value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(jlong value)
    : JxxSuper(String::valueOf(value)) {
}

AssertionError::AssertionError(
    const ::jxx::Ptr<Object>& value)
    : JxxSuper(String::valueOf(value), throwableCause(value)) {
}

AssertionError::AssertionError(const char* value)
    : JxxSuper(value) {
}

AssertionError::AssertionError(
    const ::jxx::Ptr<String>& message,
    const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(message, cause) {
}

const char* AssertionError::typeName() const noexcept {
    return "AssertionError";
}

} // namespace jxx::lang
