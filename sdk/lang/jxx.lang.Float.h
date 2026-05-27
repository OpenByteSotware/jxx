#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang
{
    class Float final : public Number {
    public:
        explicit Float(jfloat value);

        static jxx::Ptr<Float> valueOf(jfloat value);
        static jxx::Ptr<Float> valueOf(const jxx::Ptr<String> s);
        static jfloat parseFloat(const jxx::Ptr<String> s);

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
        jfloat value_;
    };
}