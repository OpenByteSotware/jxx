#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueConverter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::metadata {
EnumValueConverter::EnumValueConverter(const ::jxx::Ptr<EnumValueMap>& values)
    : values_(values) {
    if (values_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::Object> EnumValueConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return values_->fromValue(text);
}
} // namespace jxx::ext::xml::bind::metadata
