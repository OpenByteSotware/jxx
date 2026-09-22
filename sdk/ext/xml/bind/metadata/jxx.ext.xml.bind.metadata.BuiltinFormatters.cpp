#include "ext/xml/bind/metadata/jxx.ext.xml.bind.metadata.BuiltinFormatters.h"

#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::ext::xml::bind::metadata {
namespace {
template <typename T>
::jxx::Ptr<T> requireValue(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    if (value == nullptr) throw ::jxx::lang::NullPointerException();
    const auto typed = std::dynamic_pointer_cast<T>(value);
    if (typed == nullptr) throw ::jxx::lang::IllegalArgumentException();
    return typed;
}
}

::jxx::Ptr<::jxx::lang::String> StringFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return requireValue<::jxx::lang::String>(value);
}

::jxx::Ptr<::jxx::lang::String> IntegerFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return ::jxx::lang::Integer::toString(
        requireValue<::jxx::lang::Integer>(value)->intValue());
}

::jxx::Ptr<::jxx::lang::String> LongFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return ::jxx::lang::Long::toString(
        requireValue<::jxx::lang::Long>(value)->longValue());
}

::jxx::Ptr<::jxx::lang::String> DoubleFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return ::jxx::lang::Double::toString(
        requireValue<::jxx::lang::Double>(value)->doubleValue());
}

::jxx::Ptr<::jxx::lang::String> BooleanFormatter::format(
    const ::jxx::Ptr<::jxx::lang::Object>& value) {
    return ::jxx::lang::Boolean::toString(
        requireValue<::jxx::lang::Boolean>(value)->booleanValue());
}

} // namespace jxx::ext::xml::bind::metadata
