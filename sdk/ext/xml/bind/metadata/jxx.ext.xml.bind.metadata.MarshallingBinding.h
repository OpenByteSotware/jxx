#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyBinding.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.PropertyReader.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.RepeatedValueReader.h"

namespace jxx::ext::xml::bind::metadata {
class MarshallingDescriptor;

class MarshallingBinding final
    : public ::jxx::lang::ClassBase<
          MarshallingBinding,
          ::jxx::lang::Object> {
public:
    MarshallingBinding(
        PropertyBinding::Kind kind,
        const ::jxx::Ptr<::jxx::lang::String>& localName,
        const ::jxx::Ptr<::jxx::lang::String>& nameSpace,
        ::jxx::lang::jbool required,
        ::jxx::lang::jbool nillable,
        ::jxx::lang::jbool repeated,
        const ::jxx::Ptr<PropertyReader>& reader,
        const ::jxx::Ptr<ValueFormatter>& formatter = nullptr,
        const ::jxx::Ptr<MarshallingDescriptor>& childDescriptor = nullptr,
        const ::jxx::Ptr<RepeatedValueReader>& repeatedReader = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& defaultValue = nullptr,
        const ::jxx::Ptr<::jxx::lang::String>& fixedValue = nullptr,
        ::jxx::lang::jbool omitDefault = false);

    PropertyBinding::Kind kind() const noexcept;
    ::jxx::Ptr<::jxx::lang::String> localName() const;
    ::jxx::Ptr<::jxx::lang::String> nameSpace() const;
    ::jxx::lang::jbool required() const noexcept;
    ::jxx::lang::jbool nillable() const noexcept;
    ::jxx::lang::jbool repeated() const noexcept;
    ::jxx::Ptr<PropertyReader> reader() const;
    ::jxx::Ptr<ValueFormatter> formatter() const;
    ::jxx::Ptr<MarshallingDescriptor> childDescriptor() const;
    ::jxx::Ptr<RepeatedValueReader> repeatedReader() const;
    ::jxx::Ptr<::jxx::lang::String> defaultValue() const;
    ::jxx::Ptr<::jxx::lang::String> fixedValue() const;
    ::jxx::lang::jbool omitDefault() const noexcept;
    ::jxx::lang::jbool isTextValue() const noexcept;

private:
    PropertyBinding::Kind kind_;
    ::jxx::Ptr<::jxx::lang::String> localName_;
    ::jxx::Ptr<::jxx::lang::String> namespace_;
    ::jxx::lang::jbool required_;
    ::jxx::lang::jbool nillable_;
    ::jxx::lang::jbool repeated_;
    ::jxx::Ptr<PropertyReader> reader_;
    ::jxx::Ptr<ValueFormatter> formatter_;
    ::jxx::Ptr<MarshallingDescriptor> childDescriptor_;
    ::jxx::Ptr<RepeatedValueReader> repeatedReader_;
    ::jxx::Ptr<::jxx::lang::String> defaultValue_;
    ::jxx::Ptr<::jxx::lang::String> fixedValue_;
    ::jxx::lang::jbool omitDefault_;
};

} // namespace jxx::ext::xml::bind::metadata
