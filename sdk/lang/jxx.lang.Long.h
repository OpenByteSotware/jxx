#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang
{
    class Long final : public Number {
    public:
        static constexpr jlong MIN_VALUE = static_cast<jlong>(0x8000000000000000LL);
        static constexpr jlong MAX_VALUE = static_cast<jlong>(0x7fffffffffffffffLL);

    public:
        explicit Long(jlong value);

        static jxx::Ptr<Long> valueOf(jlong value);
        static jxx::Ptr<Long> valueOf(const jxx::Ptr<String> s);
        static jlong parseLong(const jxx::Ptr<String> s);

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
        jlong value_;
    };
}