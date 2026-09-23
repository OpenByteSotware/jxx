#include <string>
#include "lang/jxx.lang.StringIndexOutOfBoundsException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {

::jxx::Ptr<ClassAny> StringIndexOutOfBoundsException::Class() {
    return JxxClassInfoMarker::Class();
}

StringIndexOutOfBoundsException::StringIndexOutOfBoundsException()
    : JxxSuper() {
}

StringIndexOutOfBoundsException::StringIndexOutOfBoundsException(
    const ::jxx::Ptr<String>& message)
    : JxxSuper(message) {
}

StringIndexOutOfBoundsException::StringIndexOutOfBoundsException(
    const char* message)
    : JxxSuper(message) {
}

StringIndexOutOfBoundsException::StringIndexOutOfBoundsException(
    jint index)
    : JxxSuper(::jxx::NEW<String>(
          std::string("String index out of range: ") +
          std::to_string(index))) {
}

const char*
StringIndexOutOfBoundsException::typeName() const noexcept {
    return "StringIndexOutOfBoundsException";
}

} // namespace jxx::lang
