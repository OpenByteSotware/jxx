
#include <cmath>
#include <cstdint>
#include <limits>
#include <mutex>
#include <random>
#include "lang/jxx.lang.Math.h"

#include "lang/jxx.lang.ArithmeticException.h"

namespace jxx::lang {

namespace {

template <typename Value, typename Wide>
Value checkedValue(Wide value) {
    if (value < static_cast<Wide>(std::numeric_limits<Value>::min()) ||
        value > static_cast<Wide>(std::numeric_limits<Value>::max())) {
        throw ArithmeticException("integer overflow");
    }
    return static_cast<Value>(value);
}

} // namespace

jdouble Math::sin(jdouble v) { return std::sin(v); }
jdouble Math::cos(jdouble v) { return std::cos(v); }
jdouble Math::tan(jdouble v) { return std::tan(v); }
jdouble Math::asin(jdouble v) { return std::asin(v); }
jdouble Math::acos(jdouble v) { return std::acos(v); }
jdouble Math::atan(jdouble v) { return std::atan(v); }
jdouble Math::toRadians(jdouble v) { return v / 180.0 * PI; }
jdouble Math::toDegrees(jdouble v) { return v * 180.0 / PI; }
jdouble Math::exp(jdouble v) { return std::exp(v); }
jdouble Math::log(jdouble v) { return std::log(v); }
jdouble Math::log10(jdouble v) { return std::log10(v); }
jdouble Math::sqrt(jdouble v) { return std::sqrt(v); }
jdouble Math::cbrt(jdouble v) { return std::cbrt(v); }
jdouble Math::IEEEremainder(jdouble x, jdouble y) { return std::remainder(x, y); }
jdouble Math::ceil(jdouble v) { return std::ceil(v); }
jdouble Math::floor(jdouble v) { return std::floor(v); }
jdouble Math::rint(jdouble v) { return std::rint(v); }
jdouble Math::atan2(jdouble y, jdouble x) { return std::atan2(y, x); }
jdouble Math::pow(jdouble v, jdouble e) { return std::pow(v, e); }

jint Math::round(jfloat v) {
    if (std::isnan(v)) return 0;
    if (v <= static_cast<jfloat>(std::numeric_limits<jint>::min())) return std::numeric_limits<jint>::min();
    if (v >= static_cast<jfloat>(std::numeric_limits<jint>::max())) return std::numeric_limits<jint>::max();
    return static_cast<jint>(std::floor(v + 0.5F));
}

jlong Math::round(jdouble v) {
    if (std::isnan(v)) return 0;
    if (v <= static_cast<jdouble>(std::numeric_limits<jlong>::min())) return std::numeric_limits<jlong>::min();
    if (v >= static_cast<jdouble>(std::numeric_limits<jlong>::max())) return std::numeric_limits<jlong>::max();
    return static_cast<jlong>(std::floor(v + 0.5));
}

jdouble Math::random() {
    static std::mutex mutex;
    static std::mt19937_64 engine(std::random_device{}());
    static std::uniform_real_distribution<jdouble> distribution(0.0, 1.0);
    std::lock_guard<std::mutex> lock(mutex);
    return distribution(engine);
}

jint Math::addExact(jint x, jint y) { return checkedValue<jint, std::int64_t>(static_cast<std::int64_t>(x) + y); }
jlong Math::addExact(jlong x, jlong y) {
    if ((y > 0 && x > std::numeric_limits<jlong>::max() - y) ||
        (y < 0 && x < std::numeric_limits<jlong>::min() - y)) throw ArithmeticException("long overflow");
    return x + y;
}
jint Math::subtractExact(jint x, jint y) { return checkedValue<jint, std::int64_t>(static_cast<std::int64_t>(x) - y); }
jlong Math::subtractExact(jlong x, jlong y) {
    if ((y < 0 && x > std::numeric_limits<jlong>::max() + y) ||
        (y > 0 && x < std::numeric_limits<jlong>::min() + y)) throw ArithmeticException("long overflow");
    return x - y;
}
jint Math::multiplyExact(jint x, jint y) { return checkedValue<jint, std::int64_t>(static_cast<std::int64_t>(x) * y); }
jlong Math::multiplyExact(jlong x, jlong y) {
#if defined(__SIZEOF_INT128__)
    const __int128 result = static_cast<__int128>(x) * static_cast<__int128>(y);
    if (result < std::numeric_limits<jlong>::min() || result > std::numeric_limits<jlong>::max()) throw ArithmeticException("long overflow");
    return static_cast<jlong>(result);
#else
    if (x == 0 || y == 0) return 0;
    if ((x == -1 && y == std::numeric_limits<jlong>::min()) ||
        (y == -1 && x == std::numeric_limits<jlong>::min()) ||
        x > std::numeric_limits<jlong>::max() / y ||
        x < std::numeric_limits<jlong>::min() / y) throw ArithmeticException("long overflow");
    return x * y;
#endif
}
jint Math::incrementExact(jint v) { return addExact(v, 1); }
jlong Math::incrementExact(jlong v) { return addExact(v, static_cast<jlong>(1)); }
jint Math::decrementExact(jint v) { return subtractExact(v, 1); }
jlong Math::decrementExact(jlong v) { return subtractExact(v, static_cast<jlong>(1)); }
jint Math::negateExact(jint v) { if (v == std::numeric_limits<jint>::min()) throw ArithmeticException("integer overflow"); return -v; }
jlong Math::negateExact(jlong v) { if (v == std::numeric_limits<jlong>::min()) throw ArithmeticException("long overflow"); return -v; }
jint Math::toIntExact(jlong v) { return checkedValue<jint, jlong>(v); }

jint Math::floorDiv(jint x, jint y) {
    if (y == 0) throw ArithmeticException("/ by zero");
    if (x == std::numeric_limits<jint>::min() && y == -1) return x;
    const auto quotient = static_cast<jint>(x / y);
    const auto remainder = static_cast<jint>(x % y);
    return (remainder != 0 && ((x ^ y) < 0)) ? quotient - 1 : quotient;
}

jlong Math::floorDiv(jlong x, jlong y) {
    if (y == 0) throw ArithmeticException("/ by zero");
    if (x == std::numeric_limits<jlong>::min() && y == -1) return x;
    const auto quotient = static_cast<jlong>(x / y);
    const auto remainder = static_cast<jlong>(x % y);
    return (remainder != 0 && ((x ^ y) < 0)) ? quotient - 1 : quotient;
}

jint Math::floorMod(jint x, jint y) {
    if (y == 0) throw ArithmeticException("/ by zero");
    if (x == std::numeric_limits<jint>::min() && y == -1) return 0;
    return static_cast<jint>(x - floorDiv(x, y) * y);
}

jlong Math::floorMod(jlong x, jlong y) {
    if (y == 0) throw ArithmeticException("/ by zero");
    if (x == std::numeric_limits<jlong>::min() && y == -1) return 0;
    return static_cast<jlong>(x - floorDiv(x, y) * y);
}

jint Math::abs(jint v) { return v < 0 ? -v : v; }
jlong Math::abs(jlong v) { return v < 0 ? -v : v; }
jfloat Math::abs(jfloat v) { return std::fabs(v); }
jdouble Math::abs(jdouble v) { return std::fabs(v); }
jint Math::max(jint x,jint y){return x>=y?x:y;} jlong Math::max(jlong x,jlong y){return x>=y?x:y;}
jfloat Math::max(jfloat x,jfloat y){return std::fmax(x,y);} jdouble Math::max(jdouble x,jdouble y){return std::fmax(x,y);}
jint Math::min(jint x,jint y){return x<=y?x:y;} jlong Math::min(jlong x,jlong y){return x<=y?x:y;}
jfloat Math::min(jfloat x,jfloat y){return std::fmin(x,y);} jdouble Math::min(jdouble x,jdouble y){return std::fmin(x,y);}

jdouble Math::ulp(jdouble v) { if (std::isinf(v)) return std::numeric_limits<jdouble>::infinity(); v=abs(v); return nextAfter(v,std::numeric_limits<jdouble>::infinity())-v; }
jfloat Math::ulp(jfloat v) { if (std::isinf(v)) return std::numeric_limits<jfloat>::infinity(); v=abs(v); return std::nextafter(v,std::numeric_limits<jfloat>::infinity())-v; }
jdouble Math::signum(jdouble v){return std::isnan(v)||v==0.0?v:(v>0.0?1.0:-1.0);} jfloat Math::signum(jfloat v){return std::isnan(v)||v==0.0F?v:(v>0.0F?1.0F:-1.0F);}
jdouble Math::sinh(jdouble v){return std::sinh(v);} jdouble Math::cosh(jdouble v){return std::cosh(v);} jdouble Math::tanh(jdouble v){return std::tanh(v);}
jdouble Math::hypot(jdouble x,jdouble y){return std::hypot(x,y);} jdouble Math::expm1(jdouble v){return std::expm1(v);} jdouble Math::log1p(jdouble v){return std::log1p(v);}
jdouble Math::copySign(jdouble m,jdouble s){return std::copysign(m,s);} jfloat Math::copySign(jfloat m,jfloat s){return std::copysign(m,s);}
jint Math::getExponent(jfloat v){return std::isfinite(v)&&v!=0.0F?std::ilogb(v):(v==0.0F?-127:128);} jint Math::getExponent(jdouble v){return std::isfinite(v)&&v!=0.0?std::ilogb(v):(v==0.0?-1023:1024);}
jdouble Math::nextAfter(jdouble s,jdouble d){return std::nextafter(s,d);} jfloat Math::nextAfter(jfloat s,jdouble d){return std::nextafter(s,static_cast<jfloat>(d));}
jdouble Math::nextUp(jdouble v){return std::nextafter(v,std::numeric_limits<jdouble>::infinity());} jfloat Math::nextUp(jfloat v){return std::nextafter(v,std::numeric_limits<jfloat>::infinity());}
::jxx::lang::jdouble Math::nextDown(::jxx::lang::jdouble value) {
    return std::nextafter(value, -std::numeric_limits<::jxx::lang::jdouble>::infinity());
}
::jxx::lang::jfloat Math::nextDown(::jxx::lang::jfloat value) {
    return std::nextafter(value, -std::numeric_limits<::jxx::lang::jfloat>::infinity());
}
jdouble Math::scalb(jdouble v,jint s){return std::scalbn(v,s);} jfloat Math::scalb(jfloat v,jint s){return std::scalbn(v,s);}

} // namespace jxx::lang
