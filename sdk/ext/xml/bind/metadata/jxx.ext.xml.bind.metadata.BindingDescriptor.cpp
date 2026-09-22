#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BindingDescriptor.h"

#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {
namespace {
::jxx::lang::jbool equalString(
    const ::jxx::Ptr<::jxx::lang::String>& left,
    const ::jxx::Ptr<::jxx::lang::String>& right) {
    return left == right || (left != nullptr && right != nullptr &&
        left->equals(::jxx::CAST<::jxx::lang::Object>(right)));
}
}

BindingDescriptor::BindingDescriptor(
    const ::jxx::Ptr<::jxx::lang::String>& typeName,
    const ::jxx::Ptr<::jxx::lang::String>& rootName,
    const ::jxx::Ptr<::jxx::lang::String>& rootNamespace,
    const ::jxx::Ptr<ObjectFactory>& factory,
    const ::jxx::Ptr<PropertyArray>& properties)
    : typeName_(typeName), rootName_(rootName),
      rootNamespace_(rootNamespace), factory_(factory),
      properties_(properties) {
    if (typeName_ == nullptr || rootName_ == nullptr ||
        rootNamespace_ == nullptr || factory_ == nullptr ||
        properties_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> BindingDescriptor::typeName() const { return typeName_; }
::jxx::Ptr<::jxx::lang::String> BindingDescriptor::rootName() const { return rootName_; }
::jxx::Ptr<::jxx::lang::String> BindingDescriptor::rootNamespace() const { return rootNamespace_; }
::jxx::Ptr<ObjectFactory> BindingDescriptor::factory() const { return factory_; }
::jxx::Ptr<BindingDescriptor::PropertyArray> BindingDescriptor::properties() const { return properties_; }

::jxx::Ptr<PropertyBinding> BindingDescriptor::find_(
    PropertyBinding::Kind kind,
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const {
    if (localName == nullptr || nameSpace == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
    for (::jxx::lang::jint index = 0;
         index < static_cast<::jxx::lang::jint>(properties_->length);
         ++index) {
        const auto property = (*properties_)[index];
        if (property != nullptr && property->kind() == kind &&
            equalString(property->localName(), localName) &&
            equalString(property->nameSpace(), nameSpace)) {
            return property;
        }
    }
    return nullptr;
}

::jxx::Ptr<PropertyBinding> BindingDescriptor::findElement(
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const {
    return find_(PropertyBinding::Kind::ELEMENT, localName, nameSpace);
}

::jxx::Ptr<PropertyBinding> BindingDescriptor::findAttribute(
    const ::jxx::Ptr<::jxx::lang::String>& localName,
    const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const {
    return find_(PropertyBinding::Kind::ATTRIBUTE, localName, nameSpace);
}

} // namespace jxx::ext::xml::bind::metadata
