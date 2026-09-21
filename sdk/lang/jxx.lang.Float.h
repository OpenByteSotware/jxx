#pragma once

#include <limits>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Float final : public ClassBase<Float, Number, Comparable<Float>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Float, JxxSuper, Comparable<Float>>;
    using JxxClassInfoMarker = ClassInfo<Float, JxxSuper, Comparable<Float>>;
    static constexpr jfloat MAX_VALUE = std::numeric_limits<jfloat>::max();
    static constexpr jfloat MIN_VALUE = std::numeric_limits<jfloat>::denorm_min();
    static constexpr jfloat MIN_NORMAL = std::numeric_limits<jfloat>::min();
    static constexpr jfloat POSITIVE_INFINITY = std::numeric_limits<jfloat>::infinity();
    static constexpr jfloat NEGATIVE_INFINITY = -std::numeric_limits<jfloat>::infinity();
    static constexpr jfloat NaN = std::numeric_limits<jfloat>::quiet_NaN();
    static constexpr jint SIZE = 32;
    static constexpr jint BYTES = 4;
    static constexpr jint MAX_EXPONENT = std::numeric_limits<jfloat>::max_exponent - 1;
    static constexpr jint MIN_EXPONENT = std::numeric_limits<jfloat>::min_exponent - 1;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Float(jfloat value);
    explicit Float(::jxx::lang::jdouble value);
    explicit Float(const jxx::Ptr<String>& value);
    static jxx::Ptr<Float> valueOf(jfloat value);
    static jxx::Ptr<Float> valueOf(const jxx::Ptr<String>& value);
    static jfloat parseFloat(const jxx::Ptr<String>& value);
    static jxx::Ptr<String> toString(jfloat value);
    static jxx::Ptr<String> toHexString(jfloat value);
    static jbool isNaN(jfloat value);
    static jbool isInfinite(jfloat value);
    static ::jxx::lang::jbool isFinite(::jxx::lang::jfloat value) noexcept;
    static ::jxx::lang::jfloat sum(::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept;
    static ::jxx::lang::jfloat min(::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept;
    static ::jxx::lang::jfloat max(::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept;
    jbool isNaN() const;
    jbool isInfinite() const;
    static jint floatToIntBits(jfloat value);
    static jint floatToRawIntBits(jfloat value);
    static jfloat intBitsToFloat(jint bits);
    static jint hashCode(jfloat value);
    static jint compare(jfloat left, jfloat right);
    jbyte byteValue() const override; jshort shortValue() const override;
    jint intValue() const override; jlong longValue() const override;
    jfloat floatValue() const override; jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Float>& other) const override;
private: jfloat value_;
};
} // namespace jxx::lang
