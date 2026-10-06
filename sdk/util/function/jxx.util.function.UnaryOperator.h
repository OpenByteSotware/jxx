#pragma once

#include "lang/jxx.lang.ClassInfo.h"

#include "util/function/jxx.util.function.Function.h"

namespace jxx {
namespace util {
namespace function {

template <typename T>
class UnaryOperator : public ::jxx::lang::InterfaceBase<UnaryOperator<T>, Function<T, T>> {
public:
    virtual ~UnaryOperator() = default;

    static jxx::Ptr<UnaryOperator<T>> identity() {
        class IdentityUnaryOperator : public virtual UnaryOperator<T> {
        public:
            virtual ~IdentityUnaryOperator() = default;
            virtual jxx::Ptr<T> apply(const jxx::Ptr<T>& t) override {
                return t;
            }
        };

        return jxx::Ptr<UnaryOperator<T>>(new IdentityUnaryOperator());
    }
};

} // namespace function
} // namespace util
} // namespace jxx
