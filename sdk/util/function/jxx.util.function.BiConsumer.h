#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util::function {

template <typename T, typename U>
class BiConsumer : public ::jxx::lang::InterfaceBase<BiConsumer<T, U>> {
public:
    ~BiConsumer() override = default;
    virtual void accept(const ::jxx::Ptr<T>& first, const ::jxx::Ptr<U>& second) = 0;

    ::jxx::Ptr<BiConsumer<T, U>> andThen(const ::jxx::Ptr<BiConsumer<T, U>>& after) {
        if (after == nullptr) throw ::jxx::lang::NullPointerException();
        class Chained final : public ::jxx::lang::ClassBase<Chained, ::jxx::lang::Object, BiConsumer<T, U>> {
        public:
            Chained(const ::jxx::Ptr<BiConsumer<T, U>>& first, const ::jxx::Ptr<BiConsumer<T, U>>& second)
                : first_(first), second_(second) {}
            void accept(const ::jxx::Ptr<T>& a, const ::jxx::Ptr<U>& b) override {
                first_->accept(a, b); second_->accept(a, b);
            }
        private:
            ::jxx::Ptr<BiConsumer<T, U>> first_;
            ::jxx::Ptr<BiConsumer<T, U>> second_;
        };
        return ::jxx::CAST<BiConsumer<T, U>>(::jxx::NEW<Chained>(self_(), after));
    }

private:
    ::jxx::Ptr<BiConsumer<T, U>> self_() {
        auto* object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) throw ::jxx::lang::IllegalStateException();
        auto self = ::jxx::CAST<BiConsumer<T, U>>(object->thisPtr());
        if (self == nullptr) throw ::jxx::lang::IllegalStateException();
        return self;
    }
};

} // namespace jxx::util::function
