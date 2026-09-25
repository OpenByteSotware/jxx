#include "ext/xml/transform/jxx.ext.xml.transform.TransformerConfigurationException.h"
namespace jxx::ext::xml::transform {
TransformerConfigurationException::TransformerConfigurationException() = default;
TransformerConfigurationException::TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::String>& message) : Super(message) {}
TransformerConfigurationException::TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::Throwable>& cause) : Super(cause) {}
TransformerConfigurationException::TransformerConfigurationException(const ::jxx::Ptr<::jxx::lang::String>& message, const ::jxx::Ptr<::jxx::lang::Throwable>& cause) : Super(message, cause) {}
::jxx::Ptr<::jxx::lang::Object> TransformerConfigurationException::cloneImpl() const { return ::jxx::NEW<TransformerConfigurationException>(*this); }
const char* TransformerConfigurationException::typeName() const noexcept { return "javax.xml.transform.TransformerConfigurationException"; }
}
