#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.Function.h"

namespace jxx::util::function {

template <typename T, typename U, typename R>
class BiFunction
    : public ::jxx::lang::InterfaceBase<BiFunction<T, U, R>> {
public:
    ~BiFunction() override = default;

    virtual ::jxx::Ptr<R> apply(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<U>& right) = 0;

    template <typename V>
    ::jxx::Ptr<BiFunction<T, U, V>> andThen(
        const ::jxx::Ptr<Function<R, V>>& after);
};

template <typename T, typename U, typename R, typename V>
class BiFunctionAndThen final
    : public ::jxx::lang::ClassBase<
          BiFunctionAndThen<T, U, R, V>,
          ::jxx::lang::Object,
          BiFunction<T, U, V>> {
public:
    BiFunctionAndThen(
        const ::jxx::Ptr<BiFunction<T, U, R>>& first,
        const ::jxx::Ptr<Function<R, V>>& after)
        : first_(first), after_(after) {
    }

    ::jxx::Ptr<V> apply(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<U>& right) override {
        return after_->apply(first_->apply(left, right));
    }

private:
    ::jxx::Ptr<BiFunction<T, U, R>> first_;
    ::jxx::Ptr<Function<R, V>> after_;
};

template <typename T, typename U, typename R>
template <typename V>
::jxx::Ptr<BiFunction<T, U, V>> BiFunction<T, U, R>::andThen(
    const ::jxx::Ptr<Function<R, V>>& after) {
    if (after == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto object = dynamic_cast<::jxx::lang::Object*>(this);
    if (object == nullptr) {
        throw ::jxx::lang::IllegalStateException(
            ::jxx::NEW<::jxx::lang::String>(
                "BiFunction implementation must inherit Object"));
    }

    auto self = std::dynamic_pointer_cast<BiFunction<T, U, R>>(
        object->thisPtr());
    if (self == nullptr) {
        throw ::jxx::lang::IllegalStateException(
            ::jxx::NEW<::jxx::lang::String>(
                "BiFunction implementation must be constructed with jxx::NEW"));
    }

    auto composed = ::jxx::NEW<BiFunctionAndThen<T, U, R, V>>(
        self,
        after);
    return std::static_pointer_cast<BiFunction<T, U, V>>(composed);
}

} // namespace jxx::util::function
