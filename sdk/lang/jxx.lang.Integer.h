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
