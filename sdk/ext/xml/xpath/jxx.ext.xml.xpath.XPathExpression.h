#pragma once
#include "ext/xml/namespace/jxx.ext.xml.namespace.QName.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::ext::xml::xpath {
class XPathExpression : public ::jxx::lang::InterfaceBase<XPathExpression> {
public:
 virtual ~XPathExpression()=default;
 virtual ::jxx::Ptr<::jxx::lang::Object> evaluate(
  const ::jxx::Ptr<::jxx::lang::Object>& item,
  const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType)=0;
 virtual ::jxx::Ptr<::jxx::lang::String> evaluate(
  const ::jxx::Ptr<::jxx::lang::Object>& item)=0;
};
} // namespace jxx::ext::xml::xpath
