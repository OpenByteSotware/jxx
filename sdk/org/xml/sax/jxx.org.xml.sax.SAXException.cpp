#include "org/xml/sax/jxx.org.xml.sax.SAXException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::org::xml::sax {

::jxx::Ptr<::jxx::lang::ClassAny> SAXException::Class() {
    return JxxClassInfoMarker::Class();
}

SAXException::SAXException()
    : JxxSuper() {
}

SAXException::SAXException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : JxxSuper(message) {
}

SAXException::SAXException(
    const ::jxx::Ptr<::jxx::lang::Exception>& exception)
    : JxxSuper(exception == nullptr ? nullptr : exception->getMessage())
    , exception_(exception) {
}

SAXException::SAXException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::Exception>& exception)
    : JxxSuper(message)
    , exception_(exception) {
}

SAXException::~SAXException() = default;

::jxx::Ptr<::jxx::lang::Exception> SAXException::getException() const {
    return exception_;
}

::jxx::Ptr<::jxx::lang::Object> SAXException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(::jxx::NEW<SAXException>(*this));
}

const char* SAXException::typeName() const noexcept {
    return "SAXException";
}

} // namespace jxx::org::xml::sax
