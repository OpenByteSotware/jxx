#include "org/xml/sax/jxx.org.xml.sax.SAXParseException.h"

#include "lang/jxx.lang.ClassInfo.h"

namespace jxx::org::xml::sax {

::jxx::Ptr<::jxx::lang::ClassAny> SAXParseException::Class() {
    return JxxClassInfoMarker::Class();
}

SAXParseException::SAXParseException(
    const ::jxx::Ptr<::jxx::lang::String>& message,
    const ::jxx::Ptr<::jxx::lang::String>& publicId,
    const ::jxx::Ptr<::jxx::lang::String>& systemId,
    ::jxx::lang::jint lineNumber,
    ::jxx::lang::jint columnNumber)
    : JxxSuper(message)
    , publicId_(publicId)
    , systemId_(systemId)
    , lineNumber_(lineNumber)
    , columnNumber_(columnNumber) {
}

SAXParseException::~SAXParseException() = default;

::jxx::Ptr<::jxx::lang::String> SAXParseException::getPublicId() const { return publicId_; }
::jxx::Ptr<::jxx::lang::String> SAXParseException::getSystemId() const { return systemId_; }
::jxx::lang::jint SAXParseException::getLineNumber() const { return lineNumber_; }
::jxx::lang::jint SAXParseException::getColumnNumber() const { return columnNumber_; }

::jxx::Ptr<::jxx::lang::Object> SAXParseException::cloneImpl() const {
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::NEW<SAXParseException>(*this));
}

const char* SAXParseException::typeName() const noexcept {
    return "SAXParseException";
}

} // namespace jxx::org::xml::sax
