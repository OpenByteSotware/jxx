#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementWrapper.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::annotation {
::jxx::Ptr<::jxx::lang::String> XmlElementWrapper::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}
XmlElementWrapper::XmlElementWrapper(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    ::jxx::lang::jbool nillable,
    ::jxx::lang::jbool required)
    : name_(name), namespace_(nameSpace), nillable_(nillable), required_(required) {
    if (name_ == nullptr || namespace_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> XmlElementWrapper::name() const { return name_; }
::jxx::Ptr<::jxx::lang::String> XmlElementWrapper::nameSpace() const { return namespace_; }
::jxx::lang::jbool XmlElementWrapper::nillable() const noexcept { return nillable_; }
::jxx::lang::jbool XmlElementWrapper::required() const noexcept { return required_; }
} // namespace jxx::ext::xml::bind::annotation
