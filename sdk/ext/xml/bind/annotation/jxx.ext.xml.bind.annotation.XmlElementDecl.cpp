#include "ext/xml/bind/annotation/jxx.ext.xml.bind.annotation.XmlElementDecl.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::annotation {

::jxx::Ptr<::jxx::lang::String> XmlElementDecl::DEFAULT() {
    return ::jxx::NEW<::jxx::lang::String>("##default");
}

XmlElementDecl::XmlElementDecl(
    const ::jxx::Ptr<::jxx::lang::String>& name,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
    const ::jxx::Ptr<::jxx::lang::ClassAny>& scope,
    const ::jxx::Ptr<::jxx::lang::String>& defaultValue,
    const ::jxx::Ptr<::jxx::lang::String>& substitutionHeadName,
    const ::jxx::Ptr<::jxx::lang::String>& substitutionHeadNamespace)
    : name_(name), namespace_(nameSpace), scope_(scope),
      defaultValue_(defaultValue), substitutionHeadName_(substitutionHeadName),
      substitutionHeadNamespace_(substitutionHeadNamespace) {
    if (name_ == nullptr || namespace_ == nullptr || defaultValue_ == nullptr ||
        substitutionHeadName_ == nullptr || substitutionHeadNamespace_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> XmlElementDecl::name() const { return name_; }
::jxx::Ptr<::jxx::lang::String> XmlElementDecl::nameSpace() const { return namespace_; }
::jxx::Ptr<::jxx::lang::ClassAny> XmlElementDecl::scope() const { return scope_; }
::jxx::Ptr<::jxx::lang::String> XmlElementDecl::defaultValue() const { return defaultValue_; }
::jxx::Ptr<::jxx::lang::String> XmlElementDecl::substitutionHeadName() const { return substitutionHeadName_; }
::jxx::Ptr<::jxx::lang::String> XmlElementDecl::substitutionHeadNamespace() const { return substitutionHeadNamespace_; }

} // namespace jxx::ext::xml::bind::annotation
