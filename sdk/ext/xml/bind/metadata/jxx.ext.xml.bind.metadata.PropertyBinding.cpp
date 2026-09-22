#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

PropertyBinding::PropertyBinding(
    Kind kind,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    ::jxx::lang::jbool required,
    ::jxx::lang::jbool nillable,
    ::jxx::lang::jbool repeated,
    const ::jxx::Ptr<PropertyWriter>& writer,
    const ::jxx::Ptr<ValueConverter>& converter,
    const ::jxx::Ptr<ObjectFactory>& childFactory,
    const ::jxx::Ptr<BindingDescriptor>& childDescriptor)
    : kind_(kind), localName_(localName), namespace_(nameSpace),
      required_(required), nillable_(nillable), repeated_(repeated),
      writer_(writer), converter_(converter), childFactory_(childFactory),
      childDescriptor_(childDescriptor) {
    if (localName_ == nullptr || nameSpace == nullptr || writer_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if ((converter_ == nullptr) == (childFactory_ == nullptr)) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    if (childFactory_ != nullptr && childDescriptor_ == nullptr) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    if (converter_ != nullptr && childDescriptor_ != nullptr) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

PropertyBinding::Kind PropertyBinding::kind() const noexcept { return kind_; }
::jxx::Ptr<::jxx::lang::String> PropertyBinding::localName() const { return localName_; }
::jxx::Ptr<::jxx::lang::String> PropertyBinding::nameSpace() const { return namespace_; }
::jxx::lang::jbool PropertyBinding::required() const noexcept { return required_; }
::jxx::lang::jbool PropertyBinding::nillable() const noexcept { return nillable_; }
::jxx::lang::jbool PropertyBinding::repeated() const noexcept { return repeated_; }
::jxx::Ptr<PropertyWriter> PropertyBinding::writer() const { return writer_; }
::jxx::Ptr<ValueConverter> PropertyBinding::converter() const { return converter_; }
::jxx::Ptr<ObjectFactory> PropertyBinding::childFactory() const { return childFactory_; }
::jxx::Ptr<BindingDescriptor> PropertyBinding::childDescriptor() const { return childDescriptor_; }
::jxx::lang::jbool PropertyBinding::isTextValue() const noexcept { return converter_ != nullptr; }
::jxx::lang::jbool PropertyBinding::isNestedObject() const noexcept { return childFactory_ != nullptr; }

} // namespace jxx::ext::xml::bind::metadata
