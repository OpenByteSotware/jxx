#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueConverter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

AdaptedValueConverter::AdaptedValueConverter(
    const ::jxx::Ptr<ValueConverter>& lexicalConverter,
    const ::jxx::Ptr<
        ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>& adapter)
    : lexicalConverter_(lexicalConverter), adapter_(adapter) {
    if (lexicalConverter_ == nullptr || adapter_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::Object> AdaptedValueConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    return adapter_->unmarshal(lexicalConverter_->convert(text));
}

} // namespace jxx::ext::xml::bind::metadata
