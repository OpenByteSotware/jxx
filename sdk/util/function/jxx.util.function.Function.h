#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace function {

template <typename T, typename R>
class Function {
public:
    virtual ~Function() = default;
    virtual jxx::Ptr<R> apply(jxx::Ptr<T> t) = 0;

    template <typename V>
    virutal jxx::Ptr<Function<V, R>> compose(jxx::Ptr<Function<V, T>> before) = 0;

    template <typename V>
    virtual jxx::Ptr<Function<T, V>> andThen(jxx::Ptr<Function<R, V>> after) = 0;

    static jxx::Ptr<Function<T, T>> identity() {
        class IdentityFunction : public virtual Function<T, T> {
        public:
            virtual ~IdentityFunction() = default;
            virtual jxx::Ptr<T> apply(jxx::Ptr<T> t) override { return t; }
        };
        return jxx::Ptr<Function<T, T>>(new IdentityFunction());
    }
};

} // namespace function
} // namespace util
} // namespace jxx
