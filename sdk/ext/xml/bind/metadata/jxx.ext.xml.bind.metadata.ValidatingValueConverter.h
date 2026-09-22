#pragma once
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LexicalValidator.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueConverter.h"
namespace jxx::ext::xml::bind::metadata {
class ValidatingValueConverter final
    : public ::jxx::lang::ClassBase<ValidatingValueConverter, ::jxx::lang::Object, ValueConverter> {
public:
    ValidatingValueConverter(
        const ::jxx::Ptr<ValueConverter>& converter,
        const ::jxx::Ptr<LexicalValidator>& validator);
    ::jxx::Ptr<::jxx::lang::Object> convert(
        const ::jxx::Ptr<::jxx::lang::String>& text) override;
private:
    ::jxx::Ptr<ValueConverter> converter_;
    ::jxx::Ptr<LexicalValidator> validator_;
};
} // namespace jxx::ext::xml::bind::metadata
