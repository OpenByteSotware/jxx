
#include <string>
#include "io/jxx.io.InvalidClassException.h"

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.String.h"

namespace jxx::io {

::jxx::Ptr<::jxx::lang::ClassAny> InvalidClassException::Class() {
    return JxxClassInfoMarker::Class();
}

InvalidClassException::InvalidClassException(
    const ::jxx::Ptr<::jxx::lang::String>& reason)
    : Super(reason) {
}

InvalidClassException::InvalidClassException(
    const ::jxx::Ptr<::jxx::lang::String>& className,
    const ::jxx::Ptr<::jxx::lang::String>& reason)
    : Super(reason)
    , classname(className) {
}

::jxx::Ptr<::jxx::lang::String>
InvalidClassException::getMessage() const {
    const auto reason = JxxSuper::getMessage();
    if (classname == nullptr) {
        return reason;
    }

    std::string message = classname->utf8();
    message += "; ";
    if (reason != nullptr) {
        message += reason->utf8();
    }
    return ::jxx::NEW<::jxx::lang::String>(message);
}

::jxx::Ptr<::jxx::lang::Object>
InvalidClassException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<InvalidClassException>(*this));
}

const char* InvalidClassException::typeName() const noexcept {
    return "InvalidClassException";
}

} // namespace jxx::io
