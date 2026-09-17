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
    static jxx::Ptr<String> toString(jlong value);
    static jint hashCode(jlong value);
    static jint compare(jlong left, jlong right);
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
