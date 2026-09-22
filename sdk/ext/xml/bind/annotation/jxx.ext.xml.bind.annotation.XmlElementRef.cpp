#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementRef.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::annotation {
::jxx::Ptr<::jxx::lang::String> XmlElementRef::DEFAULT() { return ::jxx::NEW<::jxx::lang::String>("##default"); }
XmlElementRef::XmlElementRef(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& type,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    ::jxx::lang::jbool required)
    : name_(name), type_(type), namespace_(nameSpace), required_(required) {
    if (name_ == nullptr || namespace_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> XmlElementRef::name() const { return name_; }
::jxx::Ptr<::jxx::lang::ClassAny> XmlElementRef::type() const { return type_; }
::jxx::Ptr<::jxx::lang::String> XmlElementRef::nameSpace() const { return namespace_; }
::jxx::lang::jbool XmlElementRef::required() const noexcept { return required_; }
} // namespace jxx::ext::xml::bind::annotation
