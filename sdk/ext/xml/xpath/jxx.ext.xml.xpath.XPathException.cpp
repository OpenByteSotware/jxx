#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathException.h"

namespace jxx::ext::xml::xpath {

XPathException::XPathException(
    const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {
}

XPathException::XPathException(
    const ::jxx::Ptr<::jxx::lang::Throwable>& cause)
    : Super(cause) {
}

::jxx::Ptr<::jxx::lang::Object> XPathException::cloneImpl() const {
    return ::jxx::NEW<XPathException>(*this);
}

const char* XPathException::typeName() const noexcept {
    return "jxx.ext.xml.xpath.XPathException";
}

} // namespace jxx::ext::xml::xpath
