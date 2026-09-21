#pragma once

#include <cstdint>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Short final : public ClassBase<Short, Number, Comparable<Short>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Short, JxxSuper, Comparable<Short>>;
    using JxxClassInfoMarker = ClassInfo<Short, JxxSuper, Comparable<Short>>;
    static constexpr jshort MIN_VALUE = static_cast<jshort>(-32768);
    static constexpr jshort MAX_VALUE = static_cast<jshort>(32767);
    static constexpr jint SIZE = 16;
    static constexpr jint BYTES = 2;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Short(jshort value);
    explicit Short(const jxx::Ptr<String>& value);
    static jxx::Ptr<Short> valueOf(jshort value);
    static jxx::Ptr<Short> valueOf(const jxx::Ptr<String>& value);
    static jxx::Ptr<Short> valueOf(const jxx::Ptr<String>& value, jint radix);
    static jshort parseShort(const jxx::Ptr<String>& value);
    static jshort parseShort(const jxx::Ptr<String>& value, jint radix);
    static jxx::Ptr<Short> decode(const jxx::Ptr<String>& value);
    static jxx::Ptr<String> toString(jshort value);
    static jint hashCode(jshort value);
    static jint compare(jshort left, jshort right);
    static jshort reverseBytes(jshort value);
    static ::jxx::lang::jint toUnsignedInt(
        ::jxx::lang::jshort value) noexcept;
    static ::jxx::lang::jlong toUnsignedLong(
        ::jxx::lang::jshort value) noexcept;
    jbyte byteValue() const override;
    jshort shortValue() const override;
    jint intValue() const override;
    jlong longValue() const override;
    jfloat floatValue() const override;
    jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Short>& other) const override;
private:
    jshort value_;
};
} // namespace jxx::lang
