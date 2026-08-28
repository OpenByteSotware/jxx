#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang
{
    class Integer final : public Number {
    public:
        static constexpr jint MIN_VALUE = static_cast<jint>(0x80000000);
        static constexpr jint MAX_VALUE = static_cast<jint>(0x7fffffff);

    public:
        explicit Integer(jint value);

        static jxx::Ptr<Integer> valueOf(jint value);
        static jxx::Ptr<Integer> valueOf(const jxx::Ptr<String> s);
        static jint parseInt(const jxx::Ptr<String> s);

    public:
        jbyte byteValue() const override;
        jshort shortValue() const override;
        jint intValue() const override;
        jlong longValue() const override;
        jfloat floatValue() const override;
        jdouble doubleValue() const override;

        jxx::Ptr<String> toString() const override;
        jint hashCode() const override;
        jbool equals(const jxx::Ptr<Object>& obj) const override;

    private:
        jint value_;
    };
}