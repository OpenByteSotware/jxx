#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"

namespace jxx::ext::xml::xpath {

class XPathException
    : public ::jxx::lang::ClassBase<
          XPathException,
          ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<XPathException, JxxSuper>;

    explicit XPathException(
        const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit XPathException(
        const ::jxx::Ptr<::jxx::lang::Throwable>& cause);

    ~XPathException() override = default;

protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};

} // namespace jxx::ext::xml::xpath
