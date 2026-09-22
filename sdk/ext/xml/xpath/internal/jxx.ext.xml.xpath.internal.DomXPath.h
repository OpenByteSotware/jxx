#pragma once
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPath.h"
namespace jxx::ext::xml::xpath::internal {
class DomXPath final : public ::jxx::lang::ClassBase<DomXPath,::jxx::lang::Object,::jxx::ext::xml::xpath::XPath> {
public:
 void reset() override;
 ::jxx::Ptr<::jxx::ext::xml::xpath::XPathExpression> compile(const ::jxx::Ptr<::jxx::lang::String>& expression) override;
 ::jxx::Ptr<::jxx::lang::Object> evaluate(const ::jxx::Ptr<::jxx::lang::String>& expression,const ::jxx::Ptr<::jxx::lang::Object>& item,const ::jxx::Ptr<::jxx::ext::xml::namespace_::QName>& returnType) override;
 ::jxx::Ptr<::jxx::lang::String> evaluate(const ::jxx::Ptr<::jxx::lang::String>& expression,const ::jxx::Ptr<::jxx::lang::Object>& item) override;
};
} // namespace jxx::ext::xml::xpath::internal
