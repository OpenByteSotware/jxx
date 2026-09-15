#include "lang/jxx.lang.EnumConstantNotPresentException.h"

#include <string>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {

::jxx::Ptr<String> makeMessage(
    const ::jxx::Ptr<ClassAny>& enumType,
    const ::jxx::Ptr<String>& constantName) {

    const auto typeName = enumType == nullptr
        ? nullptr
        : enumType->getName();
    return ::jxx::NEW<String>(
        (typeName == nullptr ? std::string("null") : typeName->utf8()) +
        "." +
        (constantName == nullptr
             ? std::string("null")
             : constantName->utf8()));
}

} // namespace

::jxx::Ptr<ClassAny> EnumConstantNotPresentException::Class() {
    return JxxClassInfoMarker::Class();
}

EnumConstantNotPresentException::EnumConstantNotPresentException(
    const ::jxx::Ptr<ClassAny>& enumTypeValue,
    const ::jxx::Ptr<String>& constantNameValue)
    : JxxSuper(makeMessage(enumTypeValue, constantNameValue))
    , enumType_(enumTypeValue)
    , constantName_(constantNameValue) {
}

::jxx::Ptr<ClassAny>
EnumConstantNotPresentException::enumType() const {
    return enumType_;
}

::jxx::Ptr<String>
EnumConstantNotPresentException::constantName() const {
    return constantName_;
}

const char*
EnumConstantNotPresentException::typeName() const noexcept {
    return "EnumConstantNotPresentException";
}

} // namespace jxx::lang
