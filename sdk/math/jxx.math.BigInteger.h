#pragma once
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Number.h"

namespace jxx::math
{
    static std::string narrowAscii_(const jxx::Ptr<jxx::lang::String> s);
    static jxx::Ptr<jxx::lang::String> stringFromAscii_(const std::string& s);
    static std::string stripLeadingZerosKeepOne_(const std::string& s);
    static bool allDigits_(const std::string& s);

    class BigInteger final : public jxx::lang::Number {
    public:
        BigInteger(jxx::lang::jlong value);
        explicit BigInteger(const jxx::Ptr<jxx::lang::String> value);

        static jxx::Ptr<BigInteger> valueOf(jxx::lang::jlong value);
        static jxx::Ptr<BigInteger> parse(const jxx::Ptr<jxx::lang::String> value);

    public:
        jxx::lang::jint signum() const;
        jxx::Ptr<BigInteger> abs() const;
        jxx::Ptr<BigInteger> negate() const;

        jxx::Ptr<jxx::lang::String> toString() const override;
        jxx::Ptr<jxx::lang::String> toString(jxx::lang::jint radix) const;

        jxx::lang::jbyte byteValue() const override;
        jxx::lang::jshort shortValue() const override;
        jxx::lang::jint intValue() const override;
        jxx::lang::jlong longValue() const override;
        jxx::lang::jfloat floatValue() const override;
        jxx::lang:: jdouble doubleValue() const override;

        jxx::lang::jint hashCode() const override;
        jxx::lang::jbool equals(const jxx::Ptr<jxx::lang::Object> obj) const override;

    private:
        jxx::lang::jbool negative_;
        std::string digits_; // normalized decimal digits, no sign, "0" if zero

    private:
        static void normalize_(jxx::lang::jbool& negative, std::string& digits);
        static std::string parseDigits_(const jxx::Ptr<jxx::lang::String> value, jxx::lang::jbool& negative);
        static std::string divideBySmallBase_(const std::string& dec, jxx::lang::jint base, jxx::lang::jint& remainder);
    };
}