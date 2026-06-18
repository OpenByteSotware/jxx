#pragma once

#include "util/function/jxx.util.function.Function.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class UnaryOperator : virtual public Function<T, T> {
public:
    virtual ~UnaryOperator() = default;

    static jxx::Ptr<UnaryOperator<T>> identity() {
        class IdentityUnaryOperator : public virtual UnaryOperator<T> {
        public:
            virtual ~IdentityUnaryOperator() = default;
            virtual jxx::Ptr<T> apply(jxx::Ptr<T> t) override {
                return t;
            }
        };

        return jxx::Ptr<UnaryOperator<T>>(new IdentityUnaryOperator());
    }
};

} // namespace function
} // namespace util
} // namespace jxx
