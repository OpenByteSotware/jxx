#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include "lang/jxx.lang.String.h"
#include "math/jxx.math.BigInteger.h"

namespace jxx::math
{
    static std::string narrowAscii_(const jxx::Ptr<jxx::lang::String> s)
    {
        if (!s) {
            throw std::runtime_error("null String");
        }

        const std::u16string u = s->utf16();
        std::string out;
        out.reserve(u.size());

        for (char16_t ch : u) {
            if (ch > 0x7f) {
                throw std::runtime_error("non-ascii BigInteger string not supported");
            }
            out.push_back(static_cast<char>(ch));
        }

        return out;
    }

    static jxx::Ptr<jxx::lang::String> stringFromAscii_(const std::string& s)
    {
        return jxx::NEW<jxx::lang::String>(s.c_str());
    }

    static bool isAllDigits_(const std::string& s)
    {
        for (char c : s) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return !s.empty();
    }

    static std::string stripLeadingZeros_(const std::string& s)
    {
        std::size_t i = 0;
        while (i + 1 < s.size() && s[i] == '0') {
            ++i;
        }
        return s.substr(i);
    }

    BigInteger::BigInteger(jxx::lang::jlong value)
        : negative_(value < 0),
        digits_()
    {
        if (value == 0) {
            digits_ = "0";
            negative_ = false;
            return;
        }

        unsigned long long mag =
            negative_
            ? static_cast<unsigned long long>(-(value + 1)) + 1ULL
            : static_cast<unsigned long long>(value);

        digits_ = std::to_string(mag);
        normalize_(negative_, digits_);
    }

    BigInteger::BigInteger(const jxx::Ptr<jxx::lang::String> value)
        : negative_(false),
        digits_(parseDigits_(value, negative_))
    {
        normalize_(negative_, digits_);
    }

    jxx::Ptr<BigInteger> BigInteger::valueOf(jxx::lang::jlong value)
    {
        return jxx::NEW<BigInteger>(value);
    }

    jxx::Ptr<BigInteger> BigInteger::parse(const jxx::Ptr<jxx::lang::String> value)
    {
        return jxx::NEW<BigInteger>(value);
    }

    jxx::lang::jint BigInteger::signum() const
    {
        if (digits_ == "0") {
            return 0;
        }
        return negative_ ? -1 : 1;
    }

    jxx::Ptr<BigInteger> BigInteger::abs() const
    {
        return negative_ ? jxx::NEW<BigInteger>(stringFromAscii_(digits_))
            : jxx::NEW<BigInteger>(this->toString());
    }

    jxx::Ptr<BigInteger> BigInteger::negate() const
    {
        if (digits_ == "0") {
            return jxx::NEW<BigInteger>(0LL);
        }

        std::string s = negative_ ? digits_ : ("-" + digits_);
        return jxx::NEW<BigInteger>(stringFromAscii_(s));
    }

    jxx::Ptr<jxx::lang::String> BigInteger::toString() const
    {
        return stringFromAscii_(negative_ && digits_ != "0" ? "-" + digits_ : digits_);
    }

    jxx::Ptr<jxx::lang::String> BigInteger::toString(jxx::lang::jint radix) const
    {
        if (radix < 2 || radix > 36) {
            throw std::runtime_error("radix out of range");
        }

        if (digits_ == "0") {
            return stringFromAscii_("0");
        }

        const char* alphabet = "0123456789abcdefghijklmnopqrstuvwxyz";
        std::string value = digits_;
        std::string out;

        while (value != "0") {
            jxx::lang::jint rem = 0;
            value = divideBySmallBase_(value, radix, rem);
            out.push_back(alphabet[rem]);
        }

        if (negative_) {
            out.push_back('-');
        }

        std::reverse(out.begin(), out.end());
        return stringFromAscii_(out);
    }

    jxx::lang::jbyte BigInteger::byteValue() const
    {
        return static_cast<jxx::lang::jbyte>(intValue());
    }

    jxx::lang::jshort BigInteger::shortValue() const
    {
        return static_cast<jxx::lang::jshort>(intValue());
    }

    jxx::lang::jint BigInteger::intValue() const
    {
        return static_cast<jxx::lang::jint>(longValue());
    }

    jxx::lang::jlong BigInteger::longValue() const
    {
        std::string s = negative_ ? "-" + digits_ : digits_;
        return static_cast<jxx::lang::jlong>(std::stoll(s));
    }

    jxx::lang::jfloat BigInteger::floatValue() const
    {
        std::string s = negative_ ? "-" + digits_ : digits_;
        return static_cast<jxx::lang::jfloat>(std::stof(s));
    }

    jxx::lang::jdouble BigInteger::doubleValue() const
    {
        std::string s = negative_ ? "-" + digits_ : digits_;
        return static_cast<jxx::lang::jdouble>(std::stod(s));
    }

    jxx::lang::jint BigInteger::hashCode() const
    {
        jxx::lang::jint h = 1;
        for (char c : digits_) {
            h = 31 * h + static_cast<jxx::lang::jint>(c);
        }
        return negative_ ? -h : h;
    }

    jxx::lang::jbool BigInteger::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<BigInteger> other = jxx::CAST<BigInteger, Object>(obj);
        return other ? (other->negative_ == negative_ && other->digits_ == digits_) : false;
    }

    void BigInteger::normalize_(jxx::lang::jbool& negative, std::string& digits)
    {
        if (digits.empty()) {
            digits = "0";
            negative = false;
            return;
        }

        digits = stripLeadingZeros_(digits);

        if (digits == "0") {
            negative = false;
        }
    }

    std::string BigInteger::parseDigits_(const jxx::Ptr<jxx::lang::String> value, jxx::lang::jbool& negative)
    {
        std::string s = narrowAscii_(value);
        if (s.empty()) {
            throw std::runtime_error("empty BigInteger");
        }

        std::size_t i = 0;
        negative = false;

        if (s[i] == '+' || s[i] == '-') {
            negative = (s[i] == '-');
            ++i;
        }

        if (i >= s.size()) {
            throw std::runtime_error("invalid BigInteger");
        }

        std::string digits = s.substr(i);
        if (!isAllDigits_(digits)) {
            throw std::runtime_error("invalid BigInteger digits");
        }

        return stripLeadingZeros_(digits);
    }

    std::string BigInteger::divideBySmallBase_(const std::string& dec, jxx::lang::jint base, jxx::lang::jint& remainder)
    {
        std::string q;
        q.reserve(dec.size());

        jxx::lang::jint carry = 0;
        for (char c : dec) {
            jxx::lang::jint cur = carry * 10 + (c - '0');
            jxx::lang::jint digit = cur / base;
            carry = cur % base;

            if (!q.empty() || digit != 0) {
                q.push_back(static_cast<char>('0' + digit));
            }
        }

        if (q.empty()) {
            q = "0";
        }

        remainder = carry;
        return q;
    }
}