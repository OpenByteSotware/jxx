#pragma once

#include <limits>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Double final : public ClassBase<Double, Number, Comparable<Double>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Double, JxxSuper, Comparable<Double>>;
    using JxxClassInfoMarker = ClassInfo<Double, JxxSuper, Comparable<Double>>;
    static constexpr jdouble MAX_VALUE = std::numeric_limits<jdouble>::max();
    static constexpr jdouble MIN_VALUE = std::numeric_limits<jdouble>::denorm_min();
    static constexpr jdouble MIN_NORMAL = std::numeric_limits<jdouble>::min();
    static constexpr jdouble POSITIVE_INFINITY = std::numeric_limits<jdouble>::infinity();
    static constexpr jdouble NEGATIVE_INFINITY = -std::numeric_limits<jdouble>::infinity();
    static constexpr jdouble NaN = std::numeric_limits<jdouble>::quiet_NaN();
    static constexpr jint SIZE = 64;
    static constexpr jint BYTES = 8;
    static constexpr jint MAX_EXPONENT = std::numeric_limits<jdouble>::max_exponent - 1;
    static constexpr jint MIN_EXPONENT = std::numeric_limits<jdouble>::min_exponent - 1;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Double(jdouble value);
    explicit Double(const jxx::Ptr<String>& value);
    static jxx::Ptr<Double> valueOf(jdouble value);
    static jxx::Ptr<Double> valueOf(const jxx::Ptr<String>& value);
    static jdouble parseDouble(const jxx::Ptr<String>& value);
    static jxx::Ptr<String> toString(jdouble value);
    static jxx::Ptr<String> toHexString(jdouble value);
    static jbool isNaN(jdouble value);
    static jbool isInfinite(jdouble value);
    static ::jxx::lang::jbool isFinite(::jxx::lang::jdouble value) noexcept;
    static ::jxx::lang::jdouble sum(::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept;
    static ::jxx::lang::jdouble min(::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept;
    static ::jxx::lang::jdouble max(::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept;
    jbool isNaN() const;
    jbool isInfinite() const;
    static jlong doubleToLongBits(jdouble value);
    static jlong doubleToRawLongBits(jdouble value);
    static jdouble longBitsToDouble(jlong bits);
    static jint hashCode(jdouble value);
    static jint compare(jdouble left, jdouble right);
    jbyte byteValue() const override; jshort shortValue() const override;
    jint intValue() const override; jlong longValue() const override;
    jfloat floatValue() const override; jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Double>& other) const override;
private: jdouble value_;
};
} // namespace jxx::lang
