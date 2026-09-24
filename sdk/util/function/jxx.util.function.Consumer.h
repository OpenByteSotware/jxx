#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.ConsumerSuper.h"

namespace jxx::util::function {

template <typename T>
class Consumer
    : public ::jxx::lang::InterfaceBase<Consumer<T>, ConsumerSuper<T>> {
public:
    ~Consumer() override = default;

    void acceptSuper(const ::jxx::Ptr<T>& value) override {
        accept(value);
    }

    virtual void accept(const ::jxx::Ptr<T>& value) {
        (void)value;
    }

    ::jxx::Ptr<Consumer<T>> andThen(
        const ::jxx::Ptr<ConsumerSuper<T>>& after) {
        if (after == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        class AndThenConsumer final
            : public ::jxx::lang::ClassBase<
                  AndThenConsumer,
                  ::jxx::lang::Object,
                  Consumer<T>> {
        public:
            AndThenConsumer(
                const ::jxx::Ptr<ConsumerSuper<T>>& first,
                const ::jxx::Ptr<ConsumerSuper<T>>& second)
                : first_(first)
                , second_(second) {
            }

            void acceptSuper(
                const ::jxx::Ptr<T>& value) override {
                first_->acceptSuper(value);
                second_->acceptSuper(value);
            }

            void accept(
                const ::jxx::Ptr<T>& value) override {
                acceptSuper(value);
            }

        private:
            ::jxx::Ptr<ConsumerSuper<T>> first_;
            ::jxx::Ptr<ConsumerSuper<T>> second_;
        };

        return ::jxx::CAST<Consumer<T>>(
            ::jxx::NEW<AndThenConsumer>(selfSuper_(), after));
    }

private:
    ::jxx::Ptr<ConsumerSuper<T>> selfSuper_() {
        auto* object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) {
            throw ::jxx::lang::IllegalStateException();
        }
        auto self = ::jxx::CAST<ConsumerSuper<T>>(object->thisPtr());
        if (self == nullptr) {
            throw ::jxx::lang::IllegalStateException();
        }
        return self;
    }
};

} // namespace jxx::util::function
