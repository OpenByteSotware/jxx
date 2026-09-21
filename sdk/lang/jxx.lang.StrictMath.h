#pragma once

#include "lang/jxx_types.h"

namespace jxx::lang {

class StrictMath final {
public:
    static constexpr jdouble E = 2.7182818284590452354;
    static constexpr jdouble PI = 3.14159265358979323846;

    StrictMath() = delete;

    static jdouble sin(jdouble value);
    static jdouble cos(jdouble value);
    static jdouble tan(jdouble value);
    static jdouble asin(jdouble value);
    static jdouble acos(jdouble value);
    static jdouble atan(jdouble value);
    static jdouble toRadians(jdouble degrees);
    static jdouble toDegrees(jdouble radians);
    static jdouble exp(jdouble value);
    static jdouble log(jdouble value);
    static jdouble log10(jdouble value);
    static jdouble sqrt(jdouble value);
    static jdouble cbrt(jdouble value);
    static jdouble IEEEremainder(jdouble dividend, jdouble divisor);
    static jdouble ceil(jdouble value);
    static jdouble floor(jdouble value);
    static jdouble rint(jdouble value);
    static jdouble atan2(jdouble y, jdouble x);
    static jdouble pow(jdouble value, jdouble exponent);

    static jint round(jfloat value);
    static jlong round(jdouble value);

    static jdouble random();

    static jint addExact(jint left, jint right);
    static jlong addExact(jlong left, jlong right);
    static jint subtractExact(jint left, jint right);
    static jlong subtractExact(jlong left, jlong right);
    static jint multiplyExact(jint left, jint right);
    static jlong multiplyExact(jlong left, jlong right);
    static jint incrementExact(jint value);
    static jlong incrementExact(jlong value);
    static jint decrementExact(jint value);
    static jlong decrementExact(jlong value);
    static jint negateExact(jint value);
    static jlong negateExact(jlong value);
    static jint toIntExact(jlong value);

    static jint floorDiv(jint dividend, jint divisor);
    static jlong floorDiv(jlong dividend, jlong divisor);
    static jint floorMod(jint dividend, jint divisor);
    static jlong floorMod(jlong dividend, jlong divisor);

    static jint abs(jint value);
    static jlong abs(jlong value);
    static jfloat abs(jfloat value);
    static jdouble abs(jdouble value);

    static jint max(jint left, jint right);
    static jlong max(jlong left, jlong right);
    static jfloat max(jfloat left, jfloat right);
    static jdouble max(jdouble left, jdouble right);
    static jint min(jint left, jint right);
    static jlong min(jlong left, jlong right);
    static jfloat min(jfloat left, jfloat right);
    static jdouble min(jdouble left, jdouble right);

    static jdouble ulp(jdouble value);
    static jfloat ulp(jfloat value);
    static jdouble signum(jdouble value);
    static jfloat signum(jfloat value);
    static jdouble sinh(jdouble value);
    static jdouble cosh(jdouble value);
    static jdouble tanh(jdouble value);
    static jdouble hypot(jdouble x, jdouble y);
    static jdouble expm1(jdouble value);
    static jdouble log1p(jdouble value);
    static jdouble copySign(jdouble magnitude, jdouble sign);
    static jfloat copySign(jfloat magnitude, jfloat sign);
    static jint getExponent(jfloat value);
    static jint getExponent(jdouble value);
    static jdouble nextAfter(jdouble start, jdouble direction);
    static jfloat nextAfter(jfloat start, jdouble direction);
    static jdouble nextUp(jdouble value);
    static jfloat nextUp(jfloat value);
    static ::jxx::lang::jdouble nextDown(::jxx::lang::jdouble value);
    static ::jxx::lang::jfloat nextDown(::jxx::lang::jfloat value);
    static jdouble scalb(jdouble value, jint scaleFactor);
    static jfloat scalb(jfloat value, jint scaleFactor);
};

} // namespace jxx::lang
