#pragma once

#include <type_traits>
#include <utility>

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class IntConsumer;

class IntConsumerAndThen;

class IntConsumer : public ::jxx::lang::InterfaceBase<IntConsumer> {
public:
    ~IntConsumer() override = default;

    virtual void accept(::jxx::lang::jint value) = 0;

    ::jxx::Ptr<IntConsumer> andThen(
        const ::jxx::Ptr<IntConsumer>& after);

private:
    ::jxx::Ptr<IntConsumer> self_();
};

namespace int_consumer_detail {

template <typename Callable>
class CallableIntConsumer final
    : public ::jxx::lang::ClassBase<
          CallableIntConsumer<Callable>,
          ::jxx::lang::Object,
          IntConsumer> {
public:
    template <typename Source>
    explicit CallableIntConsumer(Source&& callable)
        : callable_(std::forward<Source>(callable)) {
    }

    void accept(::jxx::lang::jint value) override {
        callable_(value);
    }

private:
    Callable callable_;
};

template <typename Callable>
auto fromCallable(Callable&& callable)
    -> ::jxx::Ptr<IntConsumer> {
    using StoredCallable = std::decay_t<Callable>;
    return ::jxx::NEW<CallableIntConsumer<StoredCallable>>(
        std::forward<Callable>(callable));
}

} // namespace int_consumer_detail

class IntConsumerAndThen final
    : public ::jxx::lang::ClassBase<
          IntConsumerAndThen,
          ::jxx::lang::Object,
          IntConsumer> {
public:
    IntConsumerAndThen(
        const ::jxx::Ptr<IntConsumer>& first,
        const ::jxx::Ptr<IntConsumer>& second)
        : first_(first), second_(second) {
    }

    void accept(::jxx::lang::jint value) override {
        first_->accept(value);
        second_->accept(value);
    }

private:
    ::jxx::Ptr<IntConsumer> first_;
    ::jxx::Ptr<IntConsumer> second_;
};

inline ::jxx::Ptr<IntConsumer> IntConsumer::self_() {
    auto object = dynamic_cast<::jxx::lang::Object*>(this);
    if (object == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    auto self = std::dynamic_pointer_cast<IntConsumer>(object->thisPtr());
    if (self == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }
    return self;
}

inline ::jxx::Ptr<IntConsumer> IntConsumer::andThen(
    const ::jxx::Ptr<IntConsumer>& after) {
    if (after == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto result = ::jxx::NEW<IntConsumerAndThen>(self_(), after);
    return std::static_pointer_cast<IntConsumer>(result);
}

} // namespace jxx::util::function
