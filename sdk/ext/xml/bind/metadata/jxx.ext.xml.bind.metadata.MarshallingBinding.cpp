#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MarshallingBinding.h"

#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

MarshallingBinding::MarshallingBinding(
    PropertyBinding::Kind kind,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    ::jxx::lang::jbool required,
    ::jxx::lang::jbool nillable,
    ::jxx::lang::jbool repeated,
    const ::jxx::Ptr<PropertyReader>& reader,
    const ::jxx::Ptr<ValueFormatter>& formatter,
    const ::jxx::Ptr<MarshallingDescriptor>& childDescriptor,
    const ::jxx::Ptr<RepeatedValueReader>& repeatedReader)
    : kind_(kind), localName_(localName), namespace_(nameSpace),
      required_(required), nillable_(nillable), repeated_(repeated),
      reader_(reader), formatter_(formatter), childDescriptor_(childDescriptor),
      repeatedReader_(repeatedReader) {
    if (localName_ == nullptr || namespace_ == nullptr || reader_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    if ((formatter_ == nullptr) == (childDescriptor_ == nullptr)) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    if (repeated_ && repeatedReader_ == nullptr) {
        throw ::jxx::lang::IllegalArgumentException();
    }
    if (!repeated_ && repeatedReader_ != nullptr) {
        throw ::jxx::lang::IllegalArgumentException();
    }
}

PropertyBinding::Kind MarshallingBinding::kind() const noexcept { return kind_; }
::jxx::Ptr<::jxx::lang::String> MarshallingBinding::localName() const { return localName_; }
::jxx::Ptr<::jxx::lang::String> MarshallingBinding::nameSpace() const { return namespace_; }
::jxx::lang::jbool MarshallingBinding::required() const noexcept { return required_; }
::jxx::lang::jbool MarshallingBinding::nillable() const noexcept { return nillable_; }
::jxx::lang::jbool MarshallingBinding::repeated() const noexcept { return repeated_; }
::jxx::Ptr<PropertyReader> MarshallingBinding::reader() const { return reader_; }
::jxx::Ptr<ValueFormatter> MarshallingBinding::formatter() const { return formatter_; }
::jxx::Ptr<MarshallingDescriptor> MarshallingBinding::childDescriptor() const { return childDescriptor_; }
::jxx::Ptr<RepeatedValueReader> MarshallingBinding::repeatedReader() const { return repeatedReader_; }
::jxx::lang::jbool MarshallingBinding::isTextValue() const noexcept { return formatter_ != nullptr; }

} // namespace jxx::ext::xml::bind::metadata
