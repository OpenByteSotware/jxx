#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::util::function {

template <typename T, typename R>
class Function
    : public ::jxx::lang::InterfaceBase<Function<T, R>> {
public:
    ~Function() override = default;
    virtual ::jxx::Ptr<R> apply(const ::jxx::Ptr<T>& value) = 0;

    template <typename V>
    ::jxx::Ptr<Function<V, R>> compose(
        const ::jxx::Ptr<Function<V, T>>& before) {
        if (before == nullptr) throw ::jxx::lang::NullPointerException();
        class Composed final : public ::jxx::lang::ClassBase<Composed, ::jxx::lang::Object, Function<V, R>> {
        public:
            Composed(const ::jxx::Ptr<Function<V, T>>& first, const ::jxx::Ptr<Function<T, R>>& second)
                : first_(first), second_(second) {}
            ::jxx::Ptr<R> apply(const ::jxx::Ptr<V>& value) override {
                return second_->apply(first_->apply(value));
            }
        private:
            ::jxx::Ptr<Function<V, T>> first_;
            ::jxx::Ptr<Function<T, R>> second_;
        };
        return ::jxx::CAST<Function<V, R>>(::jxx::NEW<Composed>(before, self_()));
    }

    template <typename V>
    ::jxx::Ptr<Function<T, V>> andThen(const ::jxx::Ptr<Function<R, V>>& after) {
        if (after == nullptr) throw ::jxx::lang::NullPointerException();
        class Chained final : public ::jxx::lang::ClassBase<Chained, ::jxx::lang::Object, Function<T, V>> {
        public:
            Chained(const ::jxx::Ptr<Function<T, R>>& first, const ::jxx::Ptr<Function<R, V>>& second)
                : first_(first), second_(second) {}
            ::jxx::Ptr<V> apply(const ::jxx::Ptr<T>& value) override {
                return second_->apply(first_->apply(value));
            }
        private:
            ::jxx::Ptr<Function<T, R>> first_;
            ::jxx::Ptr<Function<R, V>> second_;
        };
        return ::jxx::CAST<Function<T, V>>(::jxx::NEW<Chained>(self_(), after));
    }

    static ::jxx::Ptr<Function<T, T>> identity() {
        class Identity final : public ::jxx::lang::ClassBase<Identity, ::jxx::lang::Object, Function<T, T>> {
        public:
            ::jxx::Ptr<T> apply(const ::jxx::Ptr<T>& value) override { return value; }
        };
        return ::jxx::CAST<Function<T, T>>(::jxx::NEW<Identity>());
    }

private:
    ::jxx::Ptr<Function<T, R>> self_() {
        auto* object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) throw ::jxx::lang::IllegalStateException();
        auto self = ::jxx::CAST<Function<T, R>>(object->thisPtr());
        if (self == nullptr) throw ::jxx::lang::IllegalStateException();
        return self;
    }
};

} // namespace jxx::util::function
