#include "math/jxx.math.BigInteger.h"

#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cctype>

namespace
{
    static std::string narrowAscii_(const jxx::Ptr<String> s)
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

    static jxx::Ptr<String> stringFromAscii_(const std::string& s)
    {
        return jxx::NEW<String>(s.c_str());
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
}

BigInteger::BigInteger(jlong value)
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

BigInteger::BigInteger(const jxx::Ptr<String> value)
    : negative_(false),
      digits_(parseDigits_(value, negative_))
{
    normalize_(negative_, digits_);
}

jxx::Ptr<BigInteger> BigInteger::valueOf(jlong value)
{
    return jxx::NEW<BigInteger>(value);
}

jxx::Ptr<BigInteger> BigInteger::parse(const jxx::Ptr<String> value)
{
    return jxx::NEW<BigInteger>(value);
}

jint BigInteger::signum() const
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

jxx::Ptr<String> BigInteger::toString() const
{
    return stringFromAscii_(negative_ && digits_ != "0" ? "-" + digits_ : digits_);
}

jxx::Ptr<String> BigInteger::toString(jint radix) const
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
        jint rem = 0;
        value = divideBySmallBase_(value, radix, rem);
        out.push_back(alphabet[rem]);
    }

    if (negative_) {
        out.push_back('-');
    }

    std::reverse(out.begin(), out.end());
    return stringFromAscii_(out);
}

jbyte BigInteger::byteValue() const
{
    return static_cast<jbyte>(intValue());
}

jshort BigInteger::shortValue() const
{
    return static_cast<jshort>(intValue());
}

jint BigInteger::intValue() const
{
    return static_cast<jint>(longValue());
}

jlong BigInteger::longValue() const
{
    std::string s = negative_ ? "-" + digits_ : digits_;
    return static_cast<jlong>(std::stoll(s));
}

jfloat BigInteger::floatValue() const
{
    std::string s = negative_ ? "-" + digits_ : digits_;
    return static_cast<jfloat>(std::stof(s));
}

jdouble BigInteger::doubleValue() const
{
    std::string s = negative_ ? "-" + digits_ : digits_;
    return static_cast<jdouble>(std::stod(s));
}

jint BigInteger::hashCode() const
{
    jint h = 1;
    for (char c : digits_) {
        h = 31 * h + static_cast<jint>(c);
    }
    return negative_ ? -h : h;
}

jbool BigInteger::equals(const jxx::Ptr<Object> obj) const
{
    jxx::Ptr<BigInteger> other = jxx::CAST<BigInteger, Object>(obj);
    return other ? (other->negative_ == negative_ && other->digits_ == digits_) : false;
}

void BigInteger::normalize_(jbool& negative, std::string& digits)
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

std::string BigInteger::parseDigits_(const jxx::Ptr<String> value, jbool& negative)
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

std::string BigInteger::divideBySmallBase_(const std::string& dec, jint base, jint& remainder)
{
    std::string q;
    q.reserve(dec.size());

    jint carry = 0;
    for (char c : dec) {
        jint cur = carry * 10 + (c - '0');
        jint digit = cur / base;
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
