#include "lang/jxx.lang.StrictMath.h"

#include "lang/jxx.lang.Math.h"

namespace jxx::lang {

jdouble StrictMath::sin(jdouble v) { return Math::sin(v); }
jdouble StrictMath::cos(jdouble v) { return Math::cos(v); }
jdouble StrictMath::tan(jdouble v) { return Math::tan(v); }
jdouble StrictMath::asin(jdouble v) { return Math::asin(v); }
jdouble StrictMath::acos(jdouble v) { return Math::acos(v); }
jdouble StrictMath::atan(jdouble v) { return Math::atan(v); }
jdouble StrictMath::toRadians(jdouble v) { return Math::toRadians(v); }
jdouble StrictMath::toDegrees(jdouble v) { return Math::toDegrees(v); }
jdouble StrictMath::exp(jdouble v) { return Math::exp(v); }
jdouble StrictMath::log(jdouble v) { return Math::log(v); }
jdouble StrictMath::log10(jdouble v) { return Math::log10(v); }
jdouble StrictMath::sqrt(jdouble v) { return Math::sqrt(v); }
jdouble StrictMath::cbrt(jdouble v) { return Math::cbrt(v); }
jdouble StrictMath::IEEEremainder(jdouble x, jdouble y) { return Math::IEEEremainder(x, y); }
jdouble StrictMath::ceil(jdouble v) { return Math::ceil(v); }
jdouble StrictMath::floor(jdouble v) { return Math::floor(v); }
jdouble StrictMath::rint(jdouble v) { return Math::rint(v); }
jdouble StrictMath::atan2(jdouble y, jdouble x) { return Math::atan2(y, x); }
jdouble StrictMath::pow(jdouble v, jdouble e) { return Math::pow(v, e); }
jint StrictMath::round(jfloat v) { return Math::round(v); }
jlong StrictMath::round(jdouble v) { return Math::round(v); }
jdouble StrictMath::random() { return Math::random(); }
jint StrictMath::addExact(jint x, jint y) { return Math::addExact(x, y); }
jlong StrictMath::addExact(jlong x, jlong y) { return Math::addExact(x, y); }
jint StrictMath::subtractExact(jint x, jint y) { return Math::subtractExact(x, y); }
jlong StrictMath::subtractExact(jlong x, jlong y) { return Math::subtractExact(x, y); }
jint StrictMath::multiplyExact(jint x, jint y) { return Math::multiplyExact(x, y); }
jlong StrictMath::multiplyExact(jlong x, jlong y) { return Math::multiplyExact(x, y); }
jint StrictMath::incrementExact(jint v) { return Math::incrementExact(v); }
jlong StrictMath::incrementExact(jlong v) { return Math::incrementExact(v); }
jint StrictMath::decrementExact(jint v) { return Math::decrementExact(v); }
jlong StrictMath::decrementExact(jlong v) { return Math::decrementExact(v); }
jint StrictMath::negateExact(jint v) { return Math::negateExact(v); }
jlong StrictMath::negateExact(jlong v) { return Math::negateExact(v); }
jint StrictMath::toIntExact(jlong v) { return Math::toIntExact(v); }
jint StrictMath::floorDiv(jint x, jint y) { return Math::floorDiv(x, y); }
jlong StrictMath::floorDiv(jlong x, jlong y) { return Math::floorDiv(x, y); }
jint StrictMath::floorMod(jint x, jint y) { return Math::floorMod(x, y); }
jlong StrictMath::floorMod(jlong x, jlong y) { return Math::floorMod(x, y); }
jint StrictMath::abs(jint v) { return Math::abs(v); }
jlong StrictMath::abs(jlong v) { return Math::abs(v); }
jfloat StrictMath::abs(jfloat v) { return Math::abs(v); }
jdouble StrictMath::abs(jdouble v) { return Math::abs(v); }
jint StrictMath::max(jint x, jint y) { return Math::max(x, y); }
jlong StrictMath::max(jlong x, jlong y) { return Math::max(x, y); }
jfloat StrictMath::max(jfloat x, jfloat y) { return Math::max(x, y); }
jdouble StrictMath::max(jdouble x, jdouble y) { return Math::max(x, y); }
jint StrictMath::min(jint x, jint y) { return Math::min(x, y); }
jlong StrictMath::min(jlong x, jlong y) { return Math::min(x, y); }
jfloat StrictMath::min(jfloat x, jfloat y) { return Math::min(x, y); }
jdouble StrictMath::min(jdouble x, jdouble y) { return Math::min(x, y); }
jdouble StrictMath::ulp(jdouble v) { return Math::ulp(v); }
jfloat StrictMath::ulp(jfloat v) { return Math::ulp(v); }
jdouble StrictMath::signum(jdouble v) { return Math::signum(v); }
jfloat StrictMath::signum(jfloat v) { return Math::signum(v); }
jdouble StrictMath::sinh(jdouble v) { return Math::sinh(v); }
jdouble StrictMath::cosh(jdouble v) { return Math::cosh(v); }
jdouble StrictMath::tanh(jdouble v) { return Math::tanh(v); }
jdouble StrictMath::hypot(jdouble x, jdouble y) { return Math::hypot(x, y); }
jdouble StrictMath::expm1(jdouble v) { return Math::expm1(v); }
jdouble StrictMath::log1p(jdouble v) { return Math::log1p(v); }
jdouble StrictMath::copySign(jdouble m, jdouble s) { return Math::copySign(m, s); }
jfloat StrictMath::copySign(jfloat m, jfloat s) { return Math::copySign(m, s); }
jint StrictMath::getExponent(jfloat v) { return Math::getExponent(v); }
jint StrictMath::getExponent(jdouble v) { return Math::getExponent(v); }
jdouble StrictMath::nextAfter(jdouble s, jdouble d) { return Math::nextAfter(s, d); }
jfloat StrictMath::nextAfter(jfloat s, jdouble d) { return Math::nextAfter(s, d); }
jdouble StrictMath::nextUp(jdouble v) { return Math::nextUp(v); }
jfloat StrictMath::nextUp(jfloat v) { return Math::nextUp(v); }
jdouble StrictMath::scalb(jdouble v, jint s) { return Math::scalb(v, s); }
jfloat StrictMath::scalb(jfloat v, jint s) { return Math::scalb(v, s); }

} // namespace jxx::lang
