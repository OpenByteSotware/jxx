#pragma once

#include <cstdint>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Long final : public ClassBase<Long, Number, Comparable<Long>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Long, JxxSuper, Comparable<Long>>;
    using JxxClassInfoMarker = ClassInfo<Long, JxxSuper, Comparable<Long>>;
    static constexpr jlong MIN_VALUE = static_cast<jlong>(INT64_MIN);
    static constexpr jlong MAX_VALUE = static_cast<jlong>(INT64_MAX);
    static constexpr jint SIZE = 64;
    static constexpr jint BYTES = 8;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Long(jlong value);
    explicit Long(const jxx::Ptr<String>& value);
    static jxx::Ptr<Long> valueOf(jlong value);
    static jxx::Ptr<Long> valueOf(const jxx::Ptr<String>& value);
    static jxx::Ptr<Long> valueOf(const jxx::Ptr<String>& value, jint radix);
    static jlong parseLong(const jxx::Ptr<String>& value);
    static jlong parseLong(const jxx::Ptr<String>& value, jint radix);
    static jxx::Ptr<Long> decode(const jxx::Ptr<String>& value);
    static ::jxx::Ptr<Long> getLong(
        const ::jxx::Ptr<String>& name);
    static ::jxx::Ptr<Long> getLong(
        const ::jxx::Ptr<String>& name,
        ::jxx::lang::jlong defaultValue);
    static ::jxx::Ptr<Long> getLong(
        const ::jxx::Ptr<String>& name,
        const ::jxx::Ptr<Long>& defaultValue);
    static jxx::Ptr<String> toString(jlong value);
    static jint hashCode(jlong value);
    static jint compare(jlong left, jlong right);
    static ::jxx::Ptr<String> toString(::jxx::lang::jlong value, ::jxx::lang::jint radix);
    static ::jxx::Ptr<String> toUnsignedString(::jxx::lang::jlong value);
    static ::jxx::Ptr<String> toUnsignedString(::jxx::lang::jlong value, ::jxx::lang::jint radix);
    static ::jxx::Ptr<String> toHexString(::jxx::lang::jlong value);
    static ::jxx::Ptr<String> toOctalString(::jxx::lang::jlong value);
    static ::jxx::Ptr<String> toBinaryString(::jxx::lang::jlong value);
    static ::jxx::lang::jlong parseUnsignedLong(const ::jxx::Ptr<String>& value);
    static ::jxx::lang::jlong parseUnsignedLong(const ::jxx::Ptr<String>& value, ::jxx::lang::jint radix);
    static ::jxx::lang::jint compareUnsigned(::jxx::lang::jlong left, ::jxx::lang::jlong right) noexcept;
    static ::jxx::lang::jlong divideUnsigned(::jxx::lang::jlong dividend, ::jxx::lang::jlong divisor);
    static ::jxx::lang::jlong remainderUnsigned(::jxx::lang::jlong dividend, ::jxx::lang::jlong divisor);
    static ::jxx::lang::jlong highestOneBit(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jlong lowestOneBit(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jint numberOfLeadingZeros(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jint numberOfTrailingZeros(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jint bitCount(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jlong rotateLeft(::jxx::lang::jlong value, ::jxx::lang::jint distance) noexcept;
    static ::jxx::lang::jlong rotateRight(::jxx::lang::jlong value, ::jxx::lang::jint distance) noexcept;
    static ::jxx::lang::jlong reverse(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jlong signum(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jlong reverseBytes(::jxx::lang::jlong value) noexcept;
    static ::jxx::lang::jlong sum(::jxx::lang::jlong left, ::jxx::lang::jlong right) noexcept;
    static ::jxx::lang::jlong min(::jxx::lang::jlong left, ::jxx::lang::jlong right) noexcept;
    static ::jxx::lang::jlong max(::jxx::lang::jlong left, ::jxx::lang::jlong right) noexcept;
    jbyte byteValue() const override;
    jshort shortValue() const override;
    jint intValue() const override;
    jlong longValue() const override;
    jfloat floatValue() const override;
    jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Long>& other) const override;
private:
    jlong value_;
};
} // namespace jxx::lang
