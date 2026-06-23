#pragma once

#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx {
namespace util {
namespace function {

template <typename T, typename U, typename R>
class BiFunction : public jxx::lang::Object {
public:
    virtual ~BiFunction() = default;
    virtual jxx::Ptr<R> apply(jxx::Ptr<T> t, jxx::Ptr<U> u) = 0;

    template <typename V>
    jxx::Ptr<BiFunction<T, U, V>> andThen(jxx::Ptr<Function<R, V>> /*after*/) {
        throw jxx::lang::UnsupportedOperationException();
    }
};

} // namespace function
} // namespace util
} // namespace jxx
