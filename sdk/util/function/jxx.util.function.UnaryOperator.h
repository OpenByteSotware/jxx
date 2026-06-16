#pragma once

#include "lang/jxx_types.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx::util::function {

    template <typename T>
    class UnaryOperator : virtual public Function<T, T> {
    public:
        virtual ~UnaryOperator() = default;

        // Java: static <T> UnaryOperator<T> identity()
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
}
