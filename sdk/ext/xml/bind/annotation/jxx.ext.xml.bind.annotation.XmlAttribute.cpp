#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlAttribute.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::annotation {
::jxx::Ptr<::jxx::lang::String> XmlAttribute::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}
XmlAttribute::XmlAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    ::jxx::lang::jbool required)
    : name_(name), namespace_(nameSpace), required_(required) {
    if (name_ == nullptr || namespace_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> XmlAttribute::name() const { return name_; }
::jxx::Ptr<::jxx::lang::String> XmlAttribute::nameSpace() const { return namespace_; }
::jxx::lang::jbool XmlAttribute::required() const noexcept { return required_; }
} // namespace jxx::ext::xml::bind::annotation
