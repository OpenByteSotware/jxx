#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util::function {

template <typename T, typename U>
class BiConsumer
    : public ::jxx::lang::InterfaceBase<BiConsumer<T, U>> {
public:
    ~BiConsumer() override = default;

    virtual void accept(
        const ::jxx::Ptr<T>& first,
        const ::jxx::Ptr<U>& second) = 0;

    ::jxx::Ptr<BiConsumer<T, U>> andThen(
        const ::jxx::Ptr<BiConsumer<T, U>>& after);
};

template <typename T, typename U>
class BiConsumerAndThen final
    : public ::jxx::lang::ClassBase<
          BiConsumerAndThen<T, U>,
          ::jxx::lang::Object,
          BiConsumer<T, U>> {
public:
    BiConsumerAndThen(
        const ::jxx::Ptr<BiConsumer<T, U>>& first,
        const ::jxx::Ptr<BiConsumer<T, U>>& second)
        : first_(first), second_(second) {
    }

    void accept(
        const ::jxx::Ptr<T>& firstValue,
        const ::jxx::Ptr<U>& secondValue) override {
        first_->accept(firstValue, secondValue);
        second_->accept(firstValue, secondValue);
    }

private:
    ::jxx::Ptr<BiConsumer<T, U>> first_;
    ::jxx::Ptr<BiConsumer<T, U>> second_;
};

template <typename T, typename U>
::jxx::Ptr<BiConsumer<T, U>> BiConsumer<T, U>::andThen(
    const ::jxx::Ptr<BiConsumer<T, U>>& after) {
    if (after == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto object = dynamic_cast<::jxx::lang::Object*>(this);
    if (object == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    auto self = std::dynamic_pointer_cast<BiConsumer<T, U>>(
        object->thisPtr());
    if (self == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    auto chained = ::jxx::NEW<BiConsumerAndThen<T, U>>(
        self,
        after);
    return std::static_pointer_cast<BiConsumer<T, U>>(chained);
}

} // namespace jxx::util::function
