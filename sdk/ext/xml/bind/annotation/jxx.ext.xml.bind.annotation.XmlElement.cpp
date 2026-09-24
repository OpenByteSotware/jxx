#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElement.h"

namespace jxx::ext::xml::bind::annotation {

XmlElement::XmlElement(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    ::jxx::lang::jbool required,
    ::jxx::lang::jbool nillable,
    const ::jxx::Ptr<::jxx::lang::String>& defaultValue)
    : Super(), name_(name), required_(required), nillable_(nillable),
      defaultValue_(defaultValue) {
}

::jxx::Ptr<::jxx::lang::String> XmlElement::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}
::jxx::Ptr<::jxx::lang::String> XmlElement::name() const { return name_; }
::jxx::lang::jbool XmlElement::required() const noexcept { return required_; }
::jxx::lang::jbool XmlElement::nillable() const noexcept { return nillable_; }
::jxx::Ptr<::jxx::lang::String> XmlElement::defaultValue() const { return defaultValue_; }

} // namespace jxx::ext::xml::bind::annotation
