#include "text/jxx.text.ParseException.h"

namespace jxx::text {

ParseException::ParseException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    ::jxx::lang::jint errorOffset)
    : Super(message), errorOffset_(errorOffset) {}

ParseException::ParseException(const ParseException& other)
    : Super(other), errorOffset_(other.errorOffset_) {}

::jxx::lang::jint ParseException::getErrorOffset() const noexcept {
    return errorOffset_;
}

::jxx::Ptr<::jxx::lang::Object> ParseException::cloneImpl() const {
    return ::jxx::NEW<ParseException>(*this);
}

const char* ParseException::typeName() const noexcept {
    return "jxx.text.ParseException";
}

} // namespace jxx::text
