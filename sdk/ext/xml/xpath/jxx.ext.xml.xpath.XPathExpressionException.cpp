#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpressionException.h"
namespace jxx::ext::xml::xpath {
XPathExpressionException::XPathExpressionException() = default;
XPathExpressionException::XPathExpressionException(const ::jxx::Ptr<::jxx::lang::String>& message)
    : Super(message) {}
} // namespace jxx::ext::xml::xpath
