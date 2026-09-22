#pragma once
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
namespace jxx::ext::xml::xpath {
class XPathFactory : public ::jxx::lang::ClassBase<XPathFactory,::jxx::lang::Object> {
public:
 static ::jxx::Ptr<XPathFactory> newInstance();
 virtual ::jxx::Ptr<XPath> newXPath();
};
} // namespace jxx::ext::xml::xpath
