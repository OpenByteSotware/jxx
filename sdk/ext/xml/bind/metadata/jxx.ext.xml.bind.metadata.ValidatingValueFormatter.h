#pragma once
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.LexicalValidator.h"
#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValueFormatter.h"
namespace jxx::ext::xml::bind::metadata {
class ValidatingValueFormatter final
    : public ::jxx::lang::ClassBase<ValidatingValueFormatter, ::jxx::lang::Object, ValueFormatter> {
public:
    ValidatingValueFormatter(
        const ::jxx::Ptr<ValueFormatter>& formatter,
        const ::jxx::Ptr<LexicalValidator>& validator);
    ::jxx::Ptr<::jxx::lang::String> format(
        const ::jxx::Ptr<::jxx::lang::Object>& value) override;
private:
    ::jxx::Ptr<ValueFormatter> formatter_;
    ::jxx::Ptr<LexicalValidator> validator_;
};
} // namespace jxx::ext::xml::bind::metadata
