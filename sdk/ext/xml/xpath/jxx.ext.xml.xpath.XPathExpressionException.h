#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "ext/xml/xpath/jxx.ext.xml.xpath.XPathException.h"
namespace jxx::ext::xml::xpath {
class XPathExpressionException
    : public ::jxx::lang::ClassBase<XPathExpressionException, XPathException> {
public:
    using JxxSuper = XPathException;
    using Super = ::jxx::lang::ClassBase<XPathExpressionException, JxxSuper>;

    explicit XPathExpressionException(const ::jxx::Ptr<::jxx::lang::String>& message);
};
} // namespace jxx::ext::xml::xpath
