#pragma once
#include "ext/xml/transform/jxx.ext.xml.transform.TransformerException.h"
namespace jxx::ext::xml::transform {
class TransformerConfigurationException final : public ::jxx::lang::ClassBase<TransformerConfigurationException, TransformerException> {
public:
    using JxxSuper = TransformerException;
    using Super = ::jxx::lang::ClassBase<TransformerConfigurationException, JxxSuper>;
    TransformerConfigurationException();
    explicit TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::String>& message);
    explicit TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
    TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause);
protected:
    ::jxx::Ptr<::jxx::lang::Object> cloneImpl() const override;
    const char* typeName() const noexcept override;
};
}
