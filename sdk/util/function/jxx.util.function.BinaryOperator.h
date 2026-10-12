#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/jxx.util.Comparator.h"
#include "util/function/jxx.util.function.BiFunction.h"

namespace jxx::util::function {

template <typename T>
class BinaryOperator
    : public ::jxx::lang::InterfaceBase<
          BinaryOperator<T>,
          BiFunction<T, T, T>> {
public:
    ~BinaryOperator() override = default;

    virtual ::jxx::Ptr<T> apply(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<T>& right) override = 0;

    static ::jxx::Ptr<BinaryOperator<T>> minBy(
        const ::jxx::Ptr<::jxx::util::Comparator<T>>& comparator);

    static ::jxx::Ptr<BinaryOperator<T>> maxBy(
        const ::jxx::Ptr<::jxx::util::Comparator<T>>& comparator);
};

template <typename T, bool Minimum>
class BinaryOperatorByComparator final
    : public ::jxx::lang::ClassBase<
          BinaryOperatorByComparator<T, Minimum>,
          ::jxx::lang::Object,
          BinaryOperator<T>> {
public:
    explicit BinaryOperatorByComparator(
        const ::jxx::Ptr<::jxx::util::Comparator<T>>& comparator)
        : comparator_(comparator) {
    }

    ::jxx::Ptr<T> apply(
        const ::jxx::Ptr<T>& left,
        const ::jxx::Ptr<T>& right) override {
        const auto comparison = comparator_->compare(left, right);
        if constexpr (Minimum) {
            return comparison <= 0 ? left : right;
        }
        return comparison >= 0 ? left : right;
    }

private:
    ::jxx::Ptr<::jxx::util::Comparator<T>> comparator_;
};

template <typename T>
::jxx::Ptr<BinaryOperator<T>> BinaryOperator<T>::minBy(
    const ::jxx::Ptr<::jxx::util::Comparator<T>>& comparator) {
    if (comparator == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto result = ::jxx::NEW<BinaryOperatorByComparator<T, true>>(
        comparator);
    return std::static_pointer_cast<BinaryOperator<T>>(result);
}

template <typename T>
::jxx::Ptr<BinaryOperator<T>> BinaryOperator<T>::maxBy(
    const ::jxx::Ptr<::jxx::util::Comparator<T>>& comparator) {
    if (comparator == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto result = ::jxx::NEW<BinaryOperatorByComparator<T, false>>(
        comparator);
    return std::static_pointer_cast<BinaryOperator<T>>(result);
}

} // namespace jxx::util::function
