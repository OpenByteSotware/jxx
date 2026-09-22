#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathConstants.h"
namespace jxx::ext::xml::xpath { namespace {
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> q(const char* local) {
 return ::jxx::NEW<::jxx::ext::xml::namespace_::QName>(
  ::jxx::NEW<::jxx::lang::String>("http://www.w3.org/1999/XSL/Transform"),
  ::jxx::NEW<::jxx::lang::String>(local));
}}
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> XPathConstants::NUMBER(){return q("NUMBER");}
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> XPathConstants::STRING(){return q("STRING");}
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> XPathConstants::BOOLEAN(){return q("BOOLEAN");}
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> XPathConstants::NODESET(){return q("NODESET");}
::jxx::Ptr<::jxx::ext::xml::namespace_::QName> XPathConstants::NODE(){return q("NODE");}
::jxx::Ptr<::jxx::lang::String> XPathConstants::DOM_OBJECT_MODEL(){return ::jxx::NEW<::jxx::lang::String>("http://java.sun.com/jaxp/xpath/dom");}
} // namespace jxx::ext::xml::xpath
