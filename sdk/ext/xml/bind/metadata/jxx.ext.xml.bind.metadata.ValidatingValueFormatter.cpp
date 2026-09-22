#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.ValidatingValueFormatter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::metadata {
ValidatingValueFormatter::ValidatingValueFormatter(
    const ::jxx::Ptr<ValueFormatter>& formatter,
    const ::jxx::Ptr<LexicalValidator>& validator)
    : formatter_(formatter), validator_(validator) {
    if (formatter_ == nullptr || validator_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> ValidatingValueFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    const auto lexical = formatter_->format(value);
    validator_->validate(lexical);
    return lexical;
}
} // namespace jxx::ext::xml::bind::metadata
