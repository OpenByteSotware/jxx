#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Byte.h"
#include "lang/jxx.lang.Short.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"
#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"
#include "lang/jxx.lang.Character.h"

#include <string>
#include <sstream>
#include <iomanip>
#include <limits>
#include <cmath>
#include <stdexcept>
#include <cstdint>

namespace jxx::lang
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
                throw std::runtime_error("non-ascii numeric string not supported");
            }
            out.push_back(static_cast<char>(ch));
        }

        return out;
    }

    static jxx::Ptr<String> stringFromAscii_(const std::string& s)
    {
        return jxx::NEW<String>(s.c_str());
    }

    static jint hashLongBits_(jlong value)
    {
        return static_cast<jint>(value ^ (value >> 32));
    }

    static jint hashFloatBits_(jfloat value)
    {
        union {
            jfloat f;
            std::uint32_t u;
        } bits{};
        bits.f = value;
        return static_cast<jint>(bits.u);
    }

    static jint hashDoubleBits_(jdouble value)
    {
        union {
            jdouble d;
            std::uint64_t u;
        } bits{};
        bits.d = value;
        return static_cast<jint>(bits.u ^ (bits.u >> 32));
    }


    Byte::Byte(jbyte value) : value_(value)
    {}

    jxx::Ptr<Byte> Byte::valueOf(jbyte value)
    {
        return jxx::NEW<Byte>(value);
    }

    jxx::Ptr<Byte> Byte::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Byte>(parseByte(s));
    }

    jbyte Byte::parseByte(const jxx::Ptr<String> s)
    {
        long v = std::stol(narrowAscii_(s));
        if (v < MIN_VALUE || v > MAX_VALUE) {
            throw std::runtime_error("Byte out of range");
        }
        return static_cast<jbyte>(v);
    }

    jbyte Byte::byteValue() const { return value_; }
    jshort Byte::shortValue() const { return static_cast<jshort>(value_); }
    jint Byte::intValue() const { return static_cast<jint>(value_); }
    jlong Byte::longValue() const { return static_cast<jlong>(value_); }
    jfloat Byte::floatValue() const { return static_cast<jfloat>(value_); }
    jdouble Byte::doubleValue() const { return static_cast<jdouble>(value_); }

    jxx::Ptr<String> Byte::toString() const
    {
        return stringFromAscii_(std::to_string(static_cast<int>(value_)));
    }

    jint Byte::hashCode() const
    {
        return static_cast<jint>(value_);
    }

    jbool Byte::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Byte> other = jxx::CAST<Byte, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    Short::Short(jshort value)
        : value_(value)
    {}

    jxx::Ptr<Short> Short::valueOf(jshort value)
    {
        return jxx::NEW<Short>(value);
    }

    jxx::Ptr<Short> Short::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Short>(parseShort(s));
    }

    jshort Short::parseShort(const jxx::Ptr<String> s)
    {
        long v = std::stol(narrowAscii_(s));
        if (v < MIN_VALUE || v > MAX_VALUE) {
            throw std::runtime_error("Short out of range");
        }
        return static_cast<jshort>(v);
    }

    jbyte Short::byteValue() const { return static_cast<jbyte>(value_); }
    jshort Short::shortValue() const { return value_; }
    jint Short::intValue() const { return static_cast<jint>(value_); }
    jlong Short::longValue() const { return static_cast<jlong>(value_); }
    jfloat Short::floatValue() const { return static_cast<jfloat>(value_); }
    jdouble Short::doubleValue() const { return static_cast<jdouble>(value_); }

    jxx::Ptr<String> Short::toString() const
    {
        return stringFromAscii_(std::to_string(static_cast<int>(value_)));
    }

    jint Short::hashCode() const
    {
        return static_cast<jint>(value_);
    }

    jbool Short::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Short> other = jxx::CAST<Short, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    Integer::Integer(jint value)
        : value_(value)
    {}

    jxx::Ptr<Integer> Integer::valueOf(jint value)
    {
        return jxx::NEW<Integer>(value);
    }

    jxx::Ptr<Integer> Integer::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Integer>(parseInt(s));
    }

    jint Integer::parseInt(const jxx::Ptr<String> s)
    {
        long long v = std::stoll(narrowAscii_(s));
        if (v < MIN_VALUE || v > MAX_VALUE) {
            throw std::runtime_error("Integer out of range");
        }
        return static_cast<jint>(v);
    }

    jbyte Integer::byteValue() const { return static_cast<jbyte>(value_); }
    jshort Integer::shortValue() const { return static_cast<jshort>(value_); }
    jint Integer::intValue() const { return value_; }
    jlong Integer::longValue() const { return static_cast<jlong>(value_); }
    jfloat Integer::floatValue() const { return static_cast<jfloat>(value_); }
    jdouble Integer::doubleValue() const { return static_cast<jdouble>(value_); }

    jxx::Ptr<String> Integer::toString() const
    {
        return stringFromAscii_(std::to_string(static_cast<long long>(value_)));
    }

    jint Integer::hashCode() const
    {
        return value_;
    }

    jbool Integer::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Integer> other = jxx::CAST<Integer, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    Long::Long(jlong value)
        : value_(value)
    {}

    jxx::Ptr<Long> Long::valueOf(jlong value)
    {
        return jxx::NEW<Long>(value);
    }

    jxx::Ptr<Long> Long::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Long>(parseLong(s));
    }

    jlong Long::parseLong(const jxx::Ptr<String> s)
    {
        return static_cast<jlong>(std::stoll(narrowAscii_(s)));
    }

    jbyte Long::byteValue() const { return static_cast<jbyte>(value_); }
    jshort Long::shortValue() const { return static_cast<jshort>(value_); }
    jint Long::intValue() const { return static_cast<jint>(value_); }
    jlong Long::longValue() const { return value_; }
    jfloat Long::floatValue() const { return static_cast<jfloat>(value_); }
    jdouble Long::doubleValue() const { return static_cast<jdouble>(value_); }

    jxx::Ptr<String> Long::toString() const
    {
        return stringFromAscii_(std::to_string(static_cast<long long>(value_)));
    }

    jint Long::hashCode() const
    {
        return hashLongBits_(value_);
    }

    jbool Long::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Long> other = jxx::CAST<Long, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    Float::Float(jfloat value)
        : value_(value)
    {}

    jxx::Ptr<Float> Float::valueOf(jfloat value)
    {
        return jxx::NEW<Float>(value);
    }

    jxx::Ptr<Float> Float::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Float>(parseFloat(s));
    }

    jfloat Float::parseFloat(const jxx::Ptr<String> s)
    {
        return static_cast<jfloat>(std::stof(narrowAscii_(s)));
    }

    jbyte Float::byteValue() const { return static_cast<jbyte>(value_); }
    jshort Float::shortValue() const { return static_cast<jshort>(value_); }
    jint Float::intValue() const { return static_cast<jint>(value_); }
    jlong Float::longValue() const { return static_cast<jlong>(value_); }
    jfloat Float::floatValue() const { return value_; }
    jdouble Float::doubleValue() const { return static_cast<jdouble>(value_); }

    jxx::Ptr<String> Float::toString() const
    {
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<jfloat>::max_digits10) << value_;
        return stringFromAscii_(oss.str());
    }

    jint Float::hashCode() const
    {
        return hashFloatBits_(value_);
    }

    jbool Float::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Float> other = jxx::CAST<Float, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    Double::Double(jdouble value)
        : value_(value)
    {}

    jxx::Ptr<Double> Double::valueOf(jdouble value)
    {
        return jxx::NEW<Double>(value);
    }

    jxx::Ptr<Double> Double::valueOf(const jxx::Ptr<String> s)
    {
        return jxx::NEW<Double>(parseDouble(s));
    }

    jdouble Double::parseDouble(const jxx::Ptr<String> s)
    {
        return static_cast<jdouble>(std::stod(narrowAscii_(s)));
    }

    jbyte Double::byteValue() const { return static_cast<jbyte>(value_); }
    jshort Double::shortValue() const { return static_cast<jshort>(value_); }
    jint Double::intValue() const { return static_cast<jint>(value_); }
    jlong Double::longValue() const { return static_cast<jlong>(value_); }
    jfloat Double::floatValue() const { return static_cast<jfloat>(value_); }
    jdouble Double::doubleValue() const { return value_; }

    jxx::Ptr<String> Double::toString() const
    {
        std::ostringstream oss;
        oss << std::setprecision(std::numeric_limits<jdouble>::max_digits10) << value_;
        return stringFromAscii_(oss.str());
    }

    jint Double::hashCode() const
    {
        return hashDoubleBits_(value_);
    }

    jbool Double::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Double> other = jxx::CAST<Double, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }
    
}