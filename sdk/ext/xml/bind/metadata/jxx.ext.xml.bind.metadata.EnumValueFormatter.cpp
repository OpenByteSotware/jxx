#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.EnumValueFormatter.h"
#include "lang/jxx.lang.NullPointerException.h"
namespace jxx::ext::xml::bind::metadata {
EnumValueFormatter::EnumValueFormatter(const ::jxx::Ptr<EnumValueMap>& values)
    : values_(values) {
    if (values_ == nullptr) throw ::jxx::lang::NullPointerException();
}
::jxx::Ptr<::jxx::lang::String> EnumValueFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    return values_->value(value);
}
} // namespace jxx::ext::xml::bind::metadata
