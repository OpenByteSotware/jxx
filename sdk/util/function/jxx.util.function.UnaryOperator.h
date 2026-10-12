#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx::util::function {

template <typename T>
class UnaryOperator
    : public ::jxx::lang::InterfaceBase<
          UnaryOperator<T>,
          Function<T, T>> {
public:
    ~UnaryOperator() override = default;

    virtual ::jxx::Ptr<T> apply(
        const ::jxx::Ptr<T>& value) override = 0;

    static ::jxx::Ptr<UnaryOperator<T>> identity();
};

template <typename T>
class IdentityUnaryOperator final
    : public ::jxx::lang::ClassBase<
          IdentityUnaryOperator<T>,
          ::jxx::lang::Object,
          UnaryOperator<T>> {
public:
    ::jxx::Ptr<T> apply(
        const ::jxx::Ptr<T>& value) override {
        return value;
    }
};

template <typename T>
::jxx::Ptr<UnaryOperator<T>> UnaryOperator<T>::identity() {
    auto result = ::jxx::NEW<IdentityUnaryOperator<T>>();
    return std::static_pointer_cast<UnaryOperator<T>>(result);
}

} // namespace jxx::util::function
