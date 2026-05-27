#pragma once

#include "lang/jxx.lang.Number.h"
#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigInteger.h"

namespace jxx::lang {
    class String;
}

namespace jxx::math {
    class BigDecimal final : public jxx::lang::Number {
    public:
        BigDecimal(jxx::lang::jlong value);
        BigDecimal(jxx::lang::jdouble value);
        explicit BigDecimal(const jxx::Ptr<jxx::lang::String> value);

        static jxx::Ptr<BigDecimal> valueOf(jxx::lang::jlong value);
        static jxx::Ptr<BigDecimal> valueOf(jxx::lang::jdouble value);
        static jxx::Ptr<BigDecimal> parse(const jxx::Ptr<jxx::lang::String> value);

    public:
        jxx::lang::jint signum() const;
        jxx::lang::jint scale() const;
        jxx::Ptr<jxx::math::BigInteger> unscaledValue() const;
        jxx::Ptr<jxx::math::BigDecimal> abs() const;
        jxx::Ptr<jxx::math::BigDecimal> negate() const;
        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::Ptr<jxx::lang::String> toPlainString() const;

        jxx::lang::jbyte byteValue() const override;
        jxx::lang::jshort shortValue() const override;
        jxx::lang::jint intValue() const override;
        jxx::lang::jlong longValue() const override;
        jxx::lang::jfloat floatValue() const override;
        jxx::lang::jdouble doubleValue() const override;
        jxx::lang::jint hashCode() const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object> obj) const override;

    private:
        jxx::lang::jbool negative_;
        std::string digits_; // unscaled digits, normalized to "0" if zero
        jxx::lang::jint scale_;

    private:
        static void normalize_(jxx::lang::jbool& negative, std::string& digits, jxx::lang::jint& scale);
        static void parse_(const jxx::Ptr<jxx::lang::String> value, jxx::lang::jbool& negative, std::string& digits, jxx::lang::jint& scale);
    };
}
