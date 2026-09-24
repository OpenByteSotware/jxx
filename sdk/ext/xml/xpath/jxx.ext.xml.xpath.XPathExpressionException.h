#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"
namespace jxx::ext::xml::xpath {
class XPathExpressionException
    : public ::jxx::lang::ClassBase<XPathExpressionException, ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<XPathExpressionException, JxxSuper>;

    XPathExpressionException();
    explicit XPathExpressionException(const ::jxx::Ptr<::jxx::lang::String>& message);
};
} // namespace jxx::ext::xml::xpath
