#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class LongConsumer;

class LongConsumerAndThen;

class LongConsumer : public ::jxx::lang::InterfaceBase<LongConsumer> {
public:
    ~LongConsumer() override = default;

    virtual void accept(::jxx::lang::jlong value) = 0;

    ::jxx::Ptr<LongConsumer> andThen(
        const ::jxx::Ptr<LongConsumer>& after);

private:
    ::jxx::Ptr<LongConsumer> self_();
};

class LongConsumerAndThen final
    : public ::jxx::lang::ClassBase<
          LongConsumerAndThen,
          ::jxx::lang::Object,
          LongConsumer> {
public:
    LongConsumerAndThen(
        const ::jxx::Ptr<LongConsumer>& first,
        const ::jxx::Ptr<LongConsumer>& second)
        : first_(first), second_(second) {
    }

    void accept(::jxx::lang::jlong value) override {
        first_->accept(value);
        second_->accept(value);
    }

private:
    ::jxx::Ptr<LongConsumer> first_;
    ::jxx::Ptr<LongConsumer> second_;
};

inline ::jxx::Ptr<LongConsumer> LongConsumer::self_() {
    auto object = dynamic_cast<::jxx::lang::Object*>(this);
    if (object == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }

    auto self = std::dynamic_pointer_cast<LongConsumer>(object->thisPtr());
    if (self == nullptr) {
        throw ::jxx::lang::IllegalStateException();
    }
    return self;
}

inline ::jxx::Ptr<LongConsumer> LongConsumer::andThen(
    const ::jxx::Ptr<LongConsumer>& after) {
    if (after == nullptr) {
        throw ::jxx::lang::NullPointerException();
    }

    auto result = ::jxx::NEW<LongConsumerAndThen>(self_(), after);
    return std::static_pointer_cast<LongConsumer>(result);
}

} // namespace jxx::util::function
