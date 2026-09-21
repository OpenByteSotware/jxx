#pragma once

#include <cstdint>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Integer final : public ClassBase<Integer, Number, Comparable<Integer>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Integer, JxxSuper, Comparable<Integer>>;
    using JxxClassInfoMarker = ClassInfo<Integer, JxxSuper, Comparable<Integer>>;
    static constexpr jint MIN_VALUE = static_cast<jint>(INT32_MIN);
    static constexpr jint MAX_VALUE = static_cast<jint>(INT32_MAX);
    static constexpr jint SIZE = 32;
    static constexpr jint BYTES = 4;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Integer(jint value);
    explicit Integer(const jxx::Ptr<String>& value);
    static jxx::Ptr<Integer> valueOf(jint value);
    static jxx::Ptr<Integer> valueOf(const jxx::Ptr<String>& value);
    static jxx::Ptr<Integer> valueOf(const jxx::Ptr<String>& value, jint radix);
    static jint parseInt(const jxx::Ptr<String>& value);
    static jint parseInt(const jxx::Ptr<String>& value, jint radix);
    static jxx::Ptr<Integer> decode(const jxx::Ptr<String>& value);
    static jxx::Ptr<String> toString(jint value);
    static jint hashCode(jint value);
    static jint compare(jint left, jint right);
    static ::jxx::Ptr<String> toString(::jxx::lang::jint value, ::jxx::lang::jint radix);
    static ::jxx::Ptr<String> toUnsignedString(::jxx::lang::jint value);
    static ::jxx::Ptr<String> toUnsignedString(::jxx::lang::jint value, ::jxx::lang::jint radix);
    static ::jxx::Ptr<String> toHexString(::jxx::lang::jint value);
    static ::jxx::Ptr<String> toOctalString(::jxx::lang::jint value);
    static ::jxx::Ptr<String> toBinaryString(::jxx::lang::jint value);
    static ::jxx::lang::jint parseUnsignedInt(const ::jxx::Ptr<String>& value);
    static ::jxx::lang::jint parseUnsignedInt(const ::jxx::Ptr<String>& value, ::jxx::lang::jint radix);
    static ::jxx::lang::jint compareUnsigned(::jxx::lang::jint left, ::jxx::lang::jint right) noexcept;
    static ::jxx::lang::jint divideUnsigned(::jxx::lang::jint dividend, ::jxx::lang::jint divisor);
    static ::jxx::lang::jint remainderUnsigned(::jxx::lang::jint dividend, ::jxx::lang::jint divisor);
    static ::jxx::lang::jint highestOneBit(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint lowestOneBit(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint numberOfLeadingZeros(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint numberOfTrailingZeros(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint bitCount(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint rotateLeft(::jxx::lang::jint value, ::jxx::lang::jint distance) noexcept;
    static ::jxx::lang::jint rotateRight(::jxx::lang::jint value, ::jxx::lang::jint distance) noexcept;
    static ::jxx::lang::jint reverse(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint signum(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint reverseBytes(::jxx::lang::jint value) noexcept;
    static ::jxx::lang::jint sum(::jxx::lang::jint left, ::jxx::lang::jint right) noexcept;
    static ::jxx::lang::jint min(::jxx::lang::jint left, ::jxx::lang::jint right) noexcept;
    static ::jxx::lang::jint max(::jxx::lang::jint left, ::jxx::lang::jint right) noexcept;
    jbyte byteValue() const override;
    jshort shortValue() const override;
    jint intValue() const override;
    jlong longValue() const override;
    jfloat floatValue() const override;
    jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Integer>& other) const override;
private:
    jint value_;
};
} // namespace jxx::lang
