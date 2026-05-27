#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include "lang/jxx.lang.String.h"
#include "jxx.math.BigDecimal.h"

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
                throw std::runtime_error("non-ascii BigDecimal string not supported");
            }
            out.push_back(static_cast<char>(ch));
        }

        return out;
    }

    static jxx::Ptr<jxx::lang::String> stringFromAscii_(const std::string& s)
    {
        return jxx::NEW<jxx::lang::String>(s.c_str());
    }

    static std::string stripLeadingZerosKeepOne_(const std::string& s)
    {
        std::size_t i = 0;
        while (i + 1 < s.size() && s[i] == '0') {
            ++i;
        }
        return s.substr(i);
    }

    static bool allDigits_(const std::string& s)
    {
        if (s.empty()) {
            return false;
        }
        for (char c : s) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return true;
    }

    BigDecimal::BigDecimal(jxx::lang::jlong value)
        : negative_(value < 0),
        digits_(),
        scale_(0)
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
        normalize_(negative_, digits_, scale_);
    }

    BigDecimal::BigDecimal(jxx::lang::jdouble value)
        : negative_(false),
        digits_(),
        scale_(0)
    {
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<jxx::lang::jdouble>::max_digits10) << value;
        parse_(stringFromAscii_(oss.str()), negative_, digits_, scale_);
        normalize_(negative_, digits_, scale_);
    }

    BigDecimal::BigDecimal(const jxx::Ptr<jxx::lang::String> value)
        : negative_(false),
        digits_(),
        scale_(0)
    {
        parse_(value, negative_, digits_, scale_);
        normalize_(negative_, digits_, scale_);
    }

    jxx::Ptr<BigDecimal> BigDecimal::valueOf(jxx::lang::jlong value)
    {
        return jxx::NEW<BigDecimal>(value);
    }

    jxx::Ptr<BigDecimal> BigDecimal::valueOf(jxx::lang::jdouble value)
    {
        return jxx::NEW<BigDecimal>(value);
    }

    jxx::Ptr<BigDecimal> BigDecimal::parse(const jxx::Ptr<jxx::lang::String> value)
    {
        return jxx::NEW<BigDecimal>(value);
    }

    jxx::lang::jint BigDecimal::signum() const
    {
        if (digits_ == "0") {
            return 0;
        }
        return negative_ ? -1 : 1;
    }

    jxx::lang::jint BigDecimal::scale() const
    {
        return scale_;
    }

    jxx::Ptr<jxx::math::BigInteger> BigDecimal::unscaledValue() const
    {
        std::string s = negative_ && digits_ != "0" ? "-" + digits_ : digits_;
        return jxx::NEW<jxx::math::BigInteger>(stringFromAscii_(s));
    }

    jxx::Ptr<BigDecimal> BigDecimal::abs() const
    {
        return negative_ ? jxx::NEW<BigDecimal>(this->toPlainString()) : jxx::NEW<BigDecimal>(this->toString());
    }

    jxx::Ptr<BigDecimal> BigDecimal::negate() const
    {
        if (digits_ == "0") {
            return jxx::NEW<BigDecimal>(0LL);
        }

        std::string s = narrowAscii_(toPlainString());
        if (!negative_) {
            s = "-" + s;
        }
        else {
            s.erase(s.begin());
        }
        return jxx::NEW<BigDecimal>(stringFromAscii_(s));
    }

    jxx::Ptr<jxx::lang::String> BigDecimal::toString() const
    {
        return toPlainString();
    }

    jxx::Ptr<jxx::lang::String> BigDecimal::toPlainString() const
    {
        if (digits_ == "0") {
            return stringFromAscii_("0");
        }

        std::string out;

        if (scale_ == 0) {
            out = digits_;
        }
        else if (static_cast<std::size_t>(scale_) >= digits_.size()) {
            out = "0.";
            out.append(static_cast<std::size_t>(scale_) - digits_.size(), '0');
            out += digits_;
        }
        else {
            const std::size_t split = digits_.size() - static_cast<std::size_t>(scale_);
            out = digits_.substr(0, split);
            out.push_back('.');
            out += digits_.substr(split);
        }

        if (negative_) {
            out.insert(out.begin(), '-');
        }

        return stringFromAscii_(out);
    }

    jxx::lang::jbyte BigDecimal::byteValue() const
    {
        return static_cast<jxx::lang::jbyte>(intValue());
    }

    jxx::lang::jshort BigDecimal::shortValue() const
    {
        return static_cast<jxx::lang::jshort>(intValue());
    }

    jxx::lang::jint BigDecimal::intValue() const
    {
        return static_cast<jxx::lang::jint>(longValue());
    }

    jxx::lang::jlong BigDecimal::longValue() const
    {
        return static_cast<jxx::lang::jlong>(doubleValue());
    }

    jxx::lang::jfloat BigDecimal::floatValue() const
    {
        return static_cast<jxx::lang::jfloat>(doubleValue());
    }

    jxx::lang::jdouble BigDecimal::doubleValue() const
    {
        return static_cast<jxx::lang::jdouble>(std::stod(narrowAscii_(toPlainString())));
    }

    jxx::lang::jint BigDecimal::hashCode() const
    {
        jxx::lang::jint h = 1;
        for (char c : digits_) {
            h = 31 * h + static_cast<jxx::lang::jint>(c);
        }
        h = 31 * h + scale_;
        return negative_ ? -h : h;
    }

    jxx::lang::jbool BigDecimal::equals(const jxx::Ptr<jxx::lang::Object> obj) const
    {
        jxx::Ptr<BigDecimal> other = jxx::CAST<BigDecimal, jxx::lang::Object>(obj);
        return other
            ? (other->negative_ == negative_ && other->digits_ == digits_ && other->scale_ == scale_)
            : false;
    }

    void BigDecimal::normalize_(jxx::lang::jbool& negative, std::string& digits, jxx::lang::jint& scale)
    {
        if (digits.empty()) {
            digits = "0";
            scale = 0;
            negative = false;
            return;
        }

        digits = stripLeadingZerosKeepOne_(digits);

        while (scale > 0 && digits.size() > 1 && digits.back() == '0') {
            digits.pop_back();
            --scale;
        }

        if (digits == "0") {
            negative = false;
            scale = 0;
        }
    }

    void BigDecimal::parse_(const jxx::Ptr<jxx::lang::String> value, jxx::lang::jbool& negative, std::string& digits, jxx::lang::jint& scale)
    {
        std::string s = narrowAscii_(value);
        if (s.empty()) {
            throw std::runtime_error("empty BigDecimal");
        }

        std::size_t i = 0;
        negative = false;
        scale = 0;

        if (s[i] == '+' || s[i] == '-') {
            negative = (s[i] == '-');
            ++i;
        }

        if (i >= s.size()) {
            throw std::runtime_error("invalid BigDecimal");
        }

        std::string whole;
        std::string frac;
        bool seenDot = false;

        for (; i < s.size(); ++i) {
            const char c = s[i];
            if (c == '.') {
                if (seenDot) {
                    throw std::runtime_error("invalid BigDecimal");
                }
                seenDot = true;
                continue;
            }

            if (c < '0' || c > '9') {
                throw std::runtime_error("invalid BigDecimal digits");
            }

            if (seenDot) {
                frac.push_back(c);
            }
            else {
                whole.push_back(c);
            }
        }

        if (whole.empty()) {
            whole = "0";
        }

        if (!allDigits_(whole) || (!frac.empty() && !allDigits_(frac))) {
            throw std::runtime_error("invalid BigDecimal");
        }

        digits = whole + frac;
        scale = static_cast<jxx::lang::jint>(frac.size());

        if (digits.empty()) {
            digits = "0";
            scale = 0;
            negative = false;
        }
    }
}