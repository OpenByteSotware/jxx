#pragma once
#include "lang/jxx.lang.Exception.h"
namespace jxx::ext::xml::xpath {
class XPathExpressionException : public ::jxx::lang::Exception {
public:
    XPathExpressionException();
    explicit XPathExpressionException(const ::jxx::Ptr<::jxx::lang::String>& message);
};
} // namespace jxx::ext::xml::xpath
