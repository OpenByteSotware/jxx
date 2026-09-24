#include "ext/xml/bind/jxx.ext.xml.bind.UnmarshalException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::bind {

::jxx::Ptr<::jxx::lang::ClassAny> UnmarshalException::Class() {
    return JxxClassInfoMarker::Class();
}

UnmarshalException::UnmarshalException()
    : Super() {}

UnmarshalException::UnmarshalException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}

::jxx::Ptr<::jxx::lang::Object> UnmarshalException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<UnmarshalException>(*this));
}

const char* UnmarshalException::typeName() const noexcept {
    return "jxx.ext.xml.bind.UnmarshalException";
}

} // namespace jxx::ext::xml::bind
