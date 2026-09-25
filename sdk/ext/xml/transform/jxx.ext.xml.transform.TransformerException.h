#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Exception.h"
#include "lang/jxx.lang.String.h"
namespace jxx::ext::xml::transform {
class TransformerException : public ::jxx::lang::ClassBase<TransformerException, ::jxx::lang::Exception> {
public:
    using JxxSuper = ::jxx::lang::Exception;
    using Super = ::jxx::lang::ClassBase<TransformerException, JxxSuper>;
    TransformerException();
    explicit TransformerException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit TransformerException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    TransformerException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
}
