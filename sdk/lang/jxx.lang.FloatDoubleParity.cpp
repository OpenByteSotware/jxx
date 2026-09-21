#include "lang/jxx.lang.Float.h"
#include "lang/jxx.lang.Double.h"

#include <cmath>

namespace jxx::lang {

Float::Float(::jxx::lang::jdouble value)
    : Super(), value_(static_cast<::jxx::lang::jfloat>(value)) {
}

::jxx::lang::jbool Float::isFinite(::jxx::lang::jfloat value) noexcept {
    return std::isfinite(value);
}

::jxx::lang::jfloat Float::sum(
    ::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept {
    return left + right;
}

::jxx::lang::jfloat Float::min(
    ::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept {
    if (std::isnan(left)) return left;
    if (std::isnan(right)) return right;
    if (left == 0.0F && right == 0.0F)
        return std::signbit(left) ? left : right;
    return left < right ? left : right;
}

::jxx::lang::jfloat Float::max(
    ::jxx::lang::jfloat left, ::jxx::lang::jfloat right) noexcept {
    if (std::isnan(left)) return left;
    if (std::isnan(right)) return right;
    if (left == 0.0F && right == 0.0F)
        return std::signbit(left) ? right : left;
    return left > right ? left : right;
}

::jxx::lang::jbool Double::isFinite(::jxx::lang::jdouble value) noexcept {
    return std::isfinite(value);
}

::jxx::lang::jdouble Double::sum(
    ::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept {
    return left + right;
}

::jxx::lang::jdouble Double::min(
    ::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept {
    if (std::isnan(left)) return left;
    if (std::isnan(right)) return right;
    if (left == 0.0 && right == 0.0)
        return std::signbit(left) ? left : right;
    return left < right ? left : right;
}

::jxx::lang::jdouble Double::max(
    ::jxx::lang::jdouble left, ::jxx::lang::jdouble right) noexcept {
    if (std::isnan(left)) return left;
    if (std::isnan(right)) return right;
    if (left == 0.0 && right == 0.0)
        return std::signbit(left) ? right : left;
    return left > right ? left : right;
}

} // namespace jxx::lang
