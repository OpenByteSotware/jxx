#pragma once
#include "jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
    class Byte final : public Number {
    public:
        static constexpr jbyte MIN_VALUE = static_cast<jbyte>(-128);
        static constexpr jbyte MAX_VALUE = static_cast<jbyte>(127);

    public:
        explicit Byte(jbyte value);

        static jxx::Ptr<Byte> valueOf(jbyte value);
        static jxx::Ptr<Byte> valueOf(const jxx::Ptr<String> s);
        static jbyte parseByte(const jxx::Ptr<String> s);

    public:
        jbyte byteValue() const override;
        jshort shortValue() const override;
        jint intValue() const override;
        jlong longValue() const override;
        jfloat floatValue() const override;
        jdouble doubleValue() const override;

        jxx::Ptr<String> toString() const override;
        jint hashCode() const override;
        jbool equals(const jxx::Ptr<Object> obj) const override;

    private:
        jbyte value_;
    };
}