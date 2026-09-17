#pragma once

#include <cstdint>
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Comparable.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx_types.h"

namespace jxx::lang {
class ClassAny;
class String;
class Byte final : public ClassBase<Byte, Number, Comparable<Byte>> {
public:
    using JxxSuper = Number;
    using Super = ClassBase<Byte, JxxSuper, Comparable<Byte>>;
    using JxxClassInfoMarker = ClassInfo<Byte, JxxSuper, Comparable<Byte>>;
    static constexpr jbyte MIN_VALUE = static_cast<jbyte>(-128);
    static constexpr jbyte MAX_VALUE = static_cast<jbyte>(127);
    static constexpr jint SIZE = 8;
    static constexpr jint BYTES = 1;
    static const jxx::Ptr<ClassAny> TYPE;
    static jxx::Ptr<ClassAny> Class();
    explicit Byte(jbyte value);
    explicit Byte(const jxx::Ptr<String>& value);
    static jxx::Ptr<Byte> valueOf(jbyte value);
    static jxx::Ptr<Byte> valueOf(const jxx::Ptr<String>& value);
    static jxx::Ptr<Byte> valueOf(const jxx::Ptr<String>& value, jint radix);
    static jbyte parseByte(const jxx::Ptr<String>& value);
    static jbyte parseByte(const jxx::Ptr<String>& value, jint radix);
    static jxx::Ptr<Byte> decode(const jxx::Ptr<String>& value);
    static jxx::Ptr<String> toString(jbyte value);
    static jint hashCode(jbyte value);
    static jint compare(jbyte left, jbyte right);
    static jint toUnsignedInt(jbyte value);
    static jlong toUnsignedLong(jbyte value);
    jbyte byteValue() const override;
    jshort shortValue() const override;
    jint intValue() const override;
    jlong longValue() const override;
    jfloat floatValue() const override;
    jdouble doubleValue() const override;
    jxx::Ptr<String> toString() const override;
    jint hashCode() const override;
    jbool equals(const jxx::Ptr<Object>& object) const override;
    jint compareTo(const jxx::Ptr<Byte>& other) const override;
private:
    jbyte value_;
};
} // namespace jxx::lang
