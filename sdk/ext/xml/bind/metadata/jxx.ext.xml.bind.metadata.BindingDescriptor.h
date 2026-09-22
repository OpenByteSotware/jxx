#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ObjectFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.MixedContentWriter.h"

namespace jxx::ext::xml::bind::metadata {

class BindingDescriptor final
    : public ::jxx::lang::ClassBase<BindingDescriptor, ::jxx::lang::Object> {
public:
    using PropertyArray =
        ::jxx::lang::JxxArray<::jxx::Ptr<PropertyBinding>, 1U>;

    BindingDescriptor(
        const ::jxx::Ptr<::jxx::lang::String>& typeName,
        const ::jxx::Ptr<::jxx::lang::String>& rootName,
        const ::jxx::Ptr<::jxx::lang::String>& rootNamespace,
        const ::jxx::Ptr<ObjectFactory>& factory,
        const ::jxx::Ptr<PropertyArray>& properties,
        const ::jxx::Ptr<MixedContentWriter>& mixedWriter = nullptr);

    ::jxx::Ptr<::jxx::lang::String> typeName() const;
    ::jxx::Ptr<::jxx::lang::String> rootName() const;
    ::jxx::Ptr<::jxx::lang::String> rootNamespace() const;
    ::jxx::Ptr<ObjectFactory> factory() const;
    ::jxx::Ptr<PropertyArray> properties() const;
    ::jxx::Ptr<MixedContentWriter> mixedWriter() const;
    ::jxx::lang::jbool isMixed() const noexcept;
    ::jxx::Ptr<PropertyBinding> findElement(
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const;
    ::jxx::Ptr<PropertyBinding> findAttribute(
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const;

private:
    ::jxx::Ptr<PropertyBinding> find_(
        PropertyBinding::Kind kind,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace) const;

    ::jxx::Ptr<::jxx::lang::String> typeName_;
    ::jxx::Ptr<::jxx::lang::String> rootName_;
    ::jxx::Ptr<::jxx::lang::String> rootNamespace_;
    ::jxx::Ptr<ObjectFactory> factory_;
    ::jxx::Ptr<PropertyArray> properties_;
    ::jxx::Ptr<MixedContentWriter> mixedWriter_;
};

} // namespace jxx::ext::xml::bind::metadata
