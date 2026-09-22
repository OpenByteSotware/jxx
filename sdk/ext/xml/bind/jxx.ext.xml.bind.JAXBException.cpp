#include "ext/xml/bind/jxx.ext.xml.bind.JAXBException.h"
#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::ext::xml::bind {

::jxx::Ptr<::jxx::lang::ClassAny> JAXBException::Class() {
    return JxxClassInfoMarker::Class();
}

JAXBException::JAXBException()
    : JxxSuper() {}

JAXBException::JAXBException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {}

JAXBException::JAXBException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : JxxSuper(message, cause) {}

::jxx::Ptr<::jxx::lang::Object> JAXBException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<JAXBException>(*this));
}

const char* JAXBException::typeName() const noexcept {
    return "jxx.ext.xml.bind.JAXBException";
}

} // namespace jxx::ext::xml::bind
