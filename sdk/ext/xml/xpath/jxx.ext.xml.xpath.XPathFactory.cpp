#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathFactory.h"
#include "ext/xml/xpath/internal/jxx.ext.xml.xpath.internal.DomXPath.h"
namespace jxx::ext::xml::xpath {
::jxx::Ptr<XPathFactory> XPathFactory::newInstance(){return ::jxx::NEW<XPathFactory>();}
::jxx::Ptr<XPath> XPathFactory::newXPath(){return ::jxx::CAST<XPath>(::jxx::NEW<internal::DomXPath>());}
} // namespace jxx::ext::xml::xpath
