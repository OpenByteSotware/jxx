#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang
{
    class Short final : public Number {
    public:
        static constexpr jshort MIN_VALUE = static_cast<jshort>(-32768);
        static constexpr jshort MAX_VALUE = static_cast<jshort>(32767);

    public:
        explicit Short(jshort value);

        static jxx::Ptr<Short> valueOf(jshort value);
        static jxx::Ptr<Short> valueOf(const jxx::Ptr<String> s);
        static jshort parseShort(const jxx::Ptr<String> s);

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
        jshort value_;
    };
}