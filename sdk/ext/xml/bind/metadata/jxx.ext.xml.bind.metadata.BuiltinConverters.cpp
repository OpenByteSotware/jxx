#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinConverters.h"

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::ext::xml::bind::metadata {

::jxx::Ptr<::jxx::lang::Object> StringConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::CAST<::jxx::lang::Object>(text);
}

::jxx::Ptr<::jxx::lang::Object> IntegerConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Integer::valueOf(text));
}

::jxx::Ptr<::jxx::lang::Object> LongConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Long::valueOf(text));
}

::jxx::Ptr<::jxx::lang::Object> DoubleConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Double::valueOf(text));
}

::jxx::Ptr<::jxx::lang::Object> BooleanConverter::convert(
    const ::jxx::Ptr<::jxx::lang::String>& text) {
    if (text == nullptr) throw ::jxx::lang::NullPointerException();
    return ::jxx::CAST<::jxx::lang::Object>(
        ::jxx::lang::Boolean::valueOf(text));
}

} // namespace jxx::ext::xml::bind::metadata
