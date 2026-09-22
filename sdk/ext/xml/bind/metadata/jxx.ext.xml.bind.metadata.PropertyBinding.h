#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ObjectFactory.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyWriter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"

namespace jxx::ext::xml::bind::metadata {

class BindingDescriptor;

class PropertyBinding final
    : public ::jxx::lang::ClassBase<PropertyBinding, ::jxx::lang::Object> {
public:
    enum class Kind { ELEMENT, ATTRIBUTE, VALUE };

    PropertyBinding(
        Kind kind,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
        ::jxx::lang::jbool required,
        ::jxx::lang::jbool nillable,
        ::jxx::lang::jbool repeated,
        const ::jxx::Ptr<PropertyWriter>& writer,
        const ::jxx::Ptr<ValueConverter>& converter = nullptr,
        const ::jxx::Ptr<ObjectFactory>& childFactory = nullptr,
        const ::jxx::Ptr<BindingDescriptor>& childDescriptor = nullptr);

    Kind kind() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> localName() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::lang::jbool required() const noexcept;
    ::jxx::lang::jbool nillable() const noexcept;
    ::jxx::lang::jbool repeated() const noexcept;
    ::jxx::Ptr<PropertyWriter> writer() const;
    ::jxx::Ptr<ValueConverter> converter() const;
    ::jxx::Ptr<ObjectFactory> childFactory() const;
    ::jxx::Ptr<BindingDescriptor> childDescriptor() const;
    ::jxx::lang::jbool isTextValue() const noexcept;
    ::jxx::lang::jbool isNestedObject() const noexcept;

private:
    Kind kind_;
    ::jxx::Ptr<::jxx::lang::String> localName_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::lang::jbool required_;
    ::jxx::lang::jbool nillable_;
    ::jxx::lang::jbool repeated_;
    ::jxx::Ptr<PropertyWriter> writer_;
    ::jxx::Ptr<ValueConverter> converter_;
    ::jxx::Ptr<ObjectFactory> childFactory_;
    ::jxx::Ptr<BindingDescriptor> childDescriptor_;
};

} // namespace jxx::ext::xml::bind::metadata
