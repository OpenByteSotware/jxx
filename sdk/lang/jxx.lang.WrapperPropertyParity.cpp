#include <cstdint>
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"


#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.System.h"

namespace jxx::lang {

::jxx::lang::jint Short::toUnsignedInt(
    ::jxx::lang::jshort value) noexcept {
    return static_cast<::jxx::lang::jint>(
        static_cast<std::uint16_t>(value));
}

::jxx::lang::jlong Short::toUnsignedLong(
    ::jxx::lang::jshort value) noexcept {
    return static_cast<::jxx::lang::jlong>(
        static_cast<std::uint16_t>(value));
}

namespace {

::jxx::Ptr<String> propertyValue(const ::jxx::Ptr<String>& name) noexcept {
    try {
        return System::getProperty(name);
    }
    catch (const NullPointerException&) {
        return nullptr;
    }
    catch (const IllegalArgumentException&) {
        return nullptr;
    }
}

} // namespace

::jxx::Ptr<Integer> Integer::getInteger(
    const ::jxx::Ptr<String>& name) {
    return getInteger(name, ::jxx::Ptr<Integer>{});
}

::jxx::Ptr<Integer> Integer::getInteger(
    const ::jxx::Ptr<String>& name,
    ::jxx::lang::jint defaultValue) {
    return getInteger(name, valueOf(defaultValue));
}

::jxx::Ptr<Integer> Integer::getInteger(
    const ::jxx::Ptr<String>& name,
    const ::jxx::Ptr<Integer>& defaultValue) {
    const auto value = propertyValue(name);
    if (value == nullptr) return defaultValue;
    try {
        return decode(value);
    }
    catch (const NumberFormatException&) {
        return defaultValue;
    }
}

::jxx::Ptr<Long> Long::getLong(
    const ::jxx::Ptr<String>& name) {
    return getLong(name, ::jxx::Ptr<Long>{});
}

::jxx::Ptr<Long> Long::getLong(
    const ::jxx::Ptr<String>& name,
    ::jxx::lang::jlong defaultValue) {
    return getLong(name, valueOf(defaultValue));
}

::jxx::Ptr<Long> Long::getLong(
    const ::jxx::Ptr<String>& name,
    const ::jxx::Ptr<Long>& defaultValue) {
    const auto value = propertyValue(name);
    if (value == nullptr) return defaultValue;
    try {
        return decode(value);
    }
    catch (const NumberFormatException&) {
        return defaultValue;
    }
}

} // namespace jxx::lang
