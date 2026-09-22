#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.AdaptedValueFormatter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

AdaptedValueFormatter::AdaptedValueFormatter(
    const ::jxx::Ptr<ValueFormatter>& lexicalFormatter,
    const ::jxx::Ptr<
        ::jxx::ext::xml::bind::annotation::adapters::XmlAdapter>& adapter)
    : lexicalFormatter_(lexicalFormatter), adapter_(adapter) {
    if (lexicalFormatter_ == nullptr || adapter_ == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }
}

::jxx::Ptr<::jxx::lang::String> AdaptedValueFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return lexicalFormatter_->format(adapter_->marshal(value));
}

} // namespace jxx::ext::xml::bind::metadata
