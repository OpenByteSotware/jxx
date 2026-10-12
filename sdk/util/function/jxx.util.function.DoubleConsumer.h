#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class DoubleConsumer;

class DoubleConsumerAndThen;

class DoubleConsumer : public ::jxx::lang::InterfaceBase<DoubleConsumer> {
public:
    ~DoubleConsumer() override = default;

    virtual void accept(::jxx::lang::jdouble value) = 0;

    ::jxx::Ptr<DoubleConsumer> andThen(
        const ::jxx::Ptr<DoubleConsumer>& after);

private:
    ::jxx::Ptr<DoubleConsumer> self_();
};

class DoubleConsumerAndThen final
    : public ::jxx::lang::ClassBase<
          DoubleConsumerAndThen,
          ::jxx::lang::Object,
          DoubleConsumer> {
public:
    DoubleConsumerAndThen(
        const ::jxx::Ptr<DoubleConsumer>& first,
        const ::jxx::Ptr<DoubleConsumer>& second)
        : first_(first), second_(second) {
    }

    void accept(::jxx::lang::jdouble value) override {
        first_->accept(value);
        second_->accept(value);
    }

private:
    ::jxx::Ptr<DoubleConsumer> first_;
    ::jxx::Ptr<DoubleConsumer> second_;
};

inline ::jxx::Ptr<DoubleConsumer> DoubleConsumer::self_() {
    auto object = dynamic_cast<::jxx::lang::Object*>(this);
    if (object == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    auto self = std::dynamic_pointer_cast<DoubleConsumer>(object->thisPtr());
    if (self == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }
    return self;
}

inline ::jxx::Ptr<DoubleConsumer> DoubleConsumer::andThen(
    const ::jxx::Ptr<DoubleConsumer>& after) {
    if (after == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto result = ::jxx::NEW<DoubleConsumerAndThen>(self_(), after);
    return std::static_pointer_cast<DoubleConsumer>(result);
}

} // namespace jxx::util::function
