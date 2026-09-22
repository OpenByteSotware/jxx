#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueConverter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::metadata {
ValidatingValueConverter::ValidatingValueConverter(
    const ::jxx::Ptr<ValueConverter>& converter,
    const ::jxx::Ptr<LexicalValidator>& validator)
    : converter_(converter), validator_(validator) {
    if (converter_ == nullptr || validator_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::Object> ValidatingValueConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    validator_->validate(text);
    return converter_->convert(text);
}
} // namespace jxx::ext::xml::bind::metadata
