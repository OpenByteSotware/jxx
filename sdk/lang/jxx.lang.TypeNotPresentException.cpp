#include "lang/jxx.lang.TypeNotPresentException.h"

#include <string>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {
::jxx::Ptr<String> makeMessage(const ::jxx::Ptr<String>& value) {
    return ::jxx::NEW<String>(
        std::string("Type ") +
        (value == nullptr ? std::string("null") : value->utf8()) +
        " not present");
}
} // namespace

::jxx::Ptr<ClassAny> TypeNotPresentException::Class() {
    return JxxClassInfoMarker::Class();
}

TypeNotPresentException::TypeNotPresentException(
    const ::jxx::Ptr<String>& typeNameValue,
    const ::jxx::Ptr<Throwable>& cause)
    : JxxSuper(makeMessage(typeNameValue), cause)
    , typeName_(typeNameValue) {
}

TypeNotPresentException::TypeNotPresentException(
    const char* typeNameValue,
    const ::jxx::Ptr<Throwable>& cause)
    : TypeNotPresentException(
          typeNameValue == nullptr ? nullptr : ::jxx::NEW<String>(typeNameValue),
          cause) {
}

::jxx::Ptr<String> TypeNotPresentException::getTypeName() const {
    return typeName_;
}

const char* TypeNotPresentException::typeName() const noexcept {
    return "TypeNotPresentException";
}

} // namespace jxx::lang
