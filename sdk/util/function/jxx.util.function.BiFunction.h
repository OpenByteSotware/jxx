#pragma once

#include "lang/jxx.lang.UnsupportedOperationException.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx {
namespace util {
namespace function {

template <typename T, typename U, typename R>
class BiFunction : public ::jxx::lang::InterfaceBase<BiFunction<T, U, R>> {
public:
    virtual ~BiFunction() = default;
    virtual ::jxx::Ptr<R> apply(
        const ::jxx::Ptr<T>& t,
        const ::jxx::Ptr<U>& u) = 0;

    template <typename V>
    ::jxx::Ptr<BiFunction<T, U, V>> andThen(
        const ::jxx::Ptr<Function<R, V>>& /*after*/) {
        throw ::jxx::lang::UnsupportedOperationException();
    }
};

} // namespace function
} // namespace util
} // namespace jxx
