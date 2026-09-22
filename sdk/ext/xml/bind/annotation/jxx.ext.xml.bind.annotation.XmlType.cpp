#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlType.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::annotation {

::jxx::Ptr<::jxx::lang::String> XmlType::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}

XmlType::XmlType(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    const ::jxx::Ptr<StringArray>& propertyOrder,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& factoryClass,
    const ::jxx::Ptr<::jxx::lang::String>& factoryMethod)
    : name_(name), namespace_(nameSpace), propertyOrder_(propertyOrder),
      factoryClass_(factoryClass), factoryMethod_(factoryMethod) {
    if (name_ == nullptr || namespace_ == nullptr || factoryMethod_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> XmlType::name() const { return name_; }
::jxx::Ptr<::jxx::lang::String> XmlType::nameSpace() const { return namespace_; }
::jxx::Ptr<XmlType::StringArray> XmlType::propertyOrder() const { return propertyOrder_; }
::jxx::Ptr<::jxx::lang::ClassAny> XmlType::factoryClass() const { return factoryClass_; }
::jxx::Ptr<::jxx::lang::String> XmlType::factoryMethod() const { return factoryMethod_; }

} // namespace jxx::ext::xml::bind::annotation
