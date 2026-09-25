#include "ext/xml/transform/jxx.ext.xml.transform.TransformerException.h"
namespace jxx::ext::xml::transform {
TransformerException::TransformerException() = default;
TransformerException::TransformerException(const ::jxx::Ptr<::jxx::lang::String>& message) : Super(message) {}
TransformerException::TransformerException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause) : Super(cause) {}
TransformerException::TransformerException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause) : Super(message, cause) {}
::jxx::Ptr<::jxx::lang::Object> TransformerException::cloneImpl() const { return ::jxx::NEW<TransformerException>(*this); }
const char* TransformerException::typeName() const noexcept { return "javax.xml.transform.TransformerException"; }
}
