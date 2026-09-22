#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlRootElement.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::annotation {

::jxx::Ptr<::jxx::lang::String> XmlRootElement::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}

XmlRootElement::XmlRootElement(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace)
    : name_(name), namespace_(nameSpace) {
    if (name_ == nullptr || namespace_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> XmlRootElement::name() const { return name_; }
::jxx::Ptr<::jxx::lang::String> XmlRootElement::nameSpace() const { return namespace_; }

} // namespace jxx::ext::xml::bind::annotation
