#pragma once
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathExpression.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::xpath {
class XPath : public ::jxx::lang::InterfaceBase<XPath> {
public:
 virtual ~XPath()=default;
 virtual void reset()=0;
 virtual ::jxx::Ptr<XPathExpression> compile(const ::jxx::Ptr<::jxx::lang::String>& expression)=0;
 virtual ::jxx::Ptr<::jxx::lang::Object> evaluate(
  const ::jxx::Ptr<::jxx::lang::String>& expression,
  const ::jxx::Ptr<::jxx::lang::Object>& item,
  const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType)=0;
 virtual ::jxx::Ptr<::jxx::lang::String> evaluate(
  const ::jxx::Ptr<::jxx::lang::String>& expression,
  const ::jxx::Ptr<::jxx::lang::Object>& item)=0;
};
} // namespace jxx::ext::xml::xpath
