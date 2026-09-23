#include <typeindex>
#include "lang/jxx.lang.Boolean.h"
#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.System.h"

namespace jxx::lang {
namespace {

::jxx::Ptr<ClassAny> registerBooleanPrimitive() {
    ClassAny::Meta metadata;
    metadata.binaryName = "boolean";
    metadata.typeId = std::type_index(typeid(::jxx::lang::jbool));
    metadata.isPrimitive = true;
    metadata.modifiers = 0x0001 | 0x0010 | 0x0400;
    return ClassAny::registerClass(metadata);
}

} // namespace

::jxx::Ptr<Boolean> Boolean::TRUE =
    ::jxx::NEW<Boolean>(true);
::jxx::Ptr<Boolean> Boolean::FALSE =
    ::jxx::NEW<Boolean>(false);
const ::jxx::Ptr<ClassAny> Boolean::TYPE =
    registerBooleanPrimitive();

::jxx::Ptr<ClassAny> Boolean::Class() {
    return JxxClassInfoMarker::Class();
}

Boolean::Boolean(::jxx::lang::jbool value)
    : Super()
    , value_(value) {
}

Boolean::Boolean(const ::jxx::Ptr<String>& value)
    : Boolean(parseBoolean(value)) {
}

::jxx::Ptr<Boolean> Boolean::valueOf(::jxx::lang::jbool value) {
    return value ? TRUE : FALSE;
}

::jxx::Ptr<Boolean> Boolean::valueOf(
    const ::jxx::Ptr<String>& value) {
    return valueOf(parseBoolean(value));
}

::jxx::lang::jbool Boolean::parseBoolean(
    const ::jxx::Ptr<String>& value) {
    return value != nullptr &&
        value->equalsIgnoreCase(
            ::jxx::NEW<String>("true"));
}

::jxx::lang::jbool Boolean::getBoolean(
    const ::jxx::Ptr<String>& name) {
    if (name == nullptr) {
        return false;
    }

    try {
        return parseBoolean(System::getProperty(name));
    }
    catch (...) {
        return false;
    }
}

::jxx::lang::jbool Boolean::booleanValue() const {
    return value_;
}

::jxx::Ptr<String> Boolean::toString() const {
    return toString(value_);
}

::jxx::Ptr<String> Boolean::toString(::jxx::lang::jbool value) {
    return ::jxx::NEW<String>(value ? "true" : "false");
}

::jxx::lang::jint Boolean::hashCode() const {
    return hashCode(value_);
}

::jxx::lang::jint Boolean::hashCode(::jxx::lang::jbool value) {
    return value ? 1231 : 1237;
}

::jxx::lang::jbool Boolean::equals(
    const ::jxx::Ptr<Object>& object) const {
    const auto other = ::jxx::CAST<Boolean>(object);
    return other != nullptr && value_ == other->value_;
}

::jxx::lang::jint Boolean::compareTo(
    const ::jxx::Ptr<Boolean>& other) const {
    if (other == nullptr) {
        throw NullPointerException();
    }
    return compare(value_, other->value_);
}

::jxx::lang::jint Boolean::compare(
    ::jxx::lang::jbool left,
    ::jxx::lang::jbool right) {
    return left == right ? 0 : left ? 1 : -1;
}

::jxx::lang::jbool Boolean::logicalAnd(
    ::jxx::lang::jbool left,
    ::jxx::lang::jbool right) noexcept {
    return left && right;
}

::jxx::lang::jbool Boolean::logicalOr(
    ::jxx::lang::jbool left,
    ::jxx::lang::jbool right) noexcept {
    return left || right;
}

::jxx::lang::jbool Boolean::logicalXor(
    ::jxx::lang::jbool left,
    ::jxx::lang::jbool right) noexcept {
    return left != right;
}

} // namespace jxx::lang
