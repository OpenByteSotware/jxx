#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/function/jxx.util.function.Consumer.h"
#include "util/function/jxx.util.function.Function.h"
#include "util/function/jxx.util.function.PredicateSuper.h"
#include "util/function/jxx.util.function.Supplier.h"

namespace jxx::util {

template <typename T>
class Optional final
    : public ::jxx::lang::ClassBase<Optional<T>, ::jxx::lang::Object> {
public:
    using Super = ::jxx::lang::ClassBase<Optional<T>, ::jxx::lang::Object>;

    static ::jxx::Ptr<Optional<T>> empty() {
        return ::jxx::NEW<Optional<T>>();
    }

    static ::jxx::Ptr<Optional<T>> of(const ::jxx::Ptr<T>& value) {
        if (value == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        return ::jxx::NEW<Optional<T>>(value);
    }

    static ::jxx::Ptr<Optional<T>> ofNullable(const ::jxx::Ptr<T>& value) {
        return value == nullptr ? empty() : of(value);
    }

    Optional() = default;

    explicit Optional(const ::jxx::Ptr<T>& value)
        : value_(value) {
    }

    ::jxx::Ptr<T> get() const {
        if (value_ == nullptr) {
            throw ::jxx::util::NoSuchElementException(
                ::jxx::NEW<::jxx::lang::String>("No value present"));
        }
        return value_;
    }

    ::jxx::lang::jbool isPresent() const noexcept {
        return value_ != nullptr;
    }

    void ifPresent(
        const ::jxx::Ptr<::jxx::util::function::Consumer<T>>& consumer) const {
        if (consumer == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (value_ != nullptr) {
            consumer->accept(value_);
        }
    }

    ::jxx::Ptr<Optional<T>> filter(
        const ::jxx::Ptr<::jxx::util::function::PredicateSuper<T>>& predicate) {
        if (predicate == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (value_ == nullptr || !predicate->test(value_)) {
            return empty();
        }
        return self_();
    }

    template <typename U>
    ::jxx::Ptr<Optional<U>> map(
        const ::jxx::Ptr<::jxx::util::function::Function<T, U>>& mapper) const {
        if (mapper == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        return value_ == nullptr
            ? Optional<U>::empty()
            : Optional<U>::ofNullable(mapper->apply(value_));
    }

    template <typename U>
    ::jxx::Ptr<Optional<U>> flatMap(
        const ::jxx::Ptr<::jxx::util::function::Function<T, Optional<U>>>& mapper) const {
        if (mapper == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        if (value_ == nullptr) {
            return Optional<U>::empty();
        }
        auto result = mapper->apply(value_);
        if (result == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        return result;
    }

    ::jxx::Ptr<T> orElse(const ::jxx::Ptr<T>& other) const {
        return value_ != nullptr ? value_ : other;
    }

    ::jxx::Ptr<T> orElseGet(
        const ::jxx::Ptr<::jxx::util::function::Supplier<T>>& supplier) const {
        if (value_ != nullptr) {
            return value_;
        }
        if (supplier == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        return supplier->get();
    }

    template <typename X>
    ::jxx::Ptr<T> orElseThrow(
        const ::jxx::Ptr<::jxx::util::function::Supplier<X>>& exceptionSupplier) const {
        if (value_ != nullptr) {
            return value_;
        }
        if (exceptionSupplier == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        auto exception = exceptionSupplier->get();
        if (exception == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }
        throw *exception;
    }

    ::jxx::lang::jbool equals(
        const ::jxx::Ptr<::jxx::lang::Object>& object) const override {
        if (object.get() == this) {
            return true;
        }
        auto other = ::jxx::CAST<Optional<T>>(object);
        if (other == nullptr) {
            return false;
        }
        if (value_ == nullptr || other->value_ == nullptr) {
            return value_ == other->value_;
        }
        auto left = ::jxx::CAST<::jxx::lang::Object>(value_);
        auto right = ::jxx::CAST<::jxx::lang::Object>(other->value_);
        return left != nullptr && right != nullptr
            ? left->equals(right)
            : value_.get() == other->value_.get();
    }

    ::jxx::lang::jint hashCode() const override {
        auto object = ::jxx::CAST<::jxx::lang::Object>(value_);
        return object == nullptr ? 0 : object->hashCode();
    }

    ::jxx::Ptr<::jxx::lang::String> toString() const override {
        if (value_ == nullptr) {
            return ::jxx::NEW<::jxx::lang::String>("Optional.empty");
        }
        auto object = ::jxx::CAST<::jxx::lang::Object>(value_);
        auto text = object == nullptr ? nullptr : object->toString();
        return ::jxx::NEW<::jxx::lang::String>(
            std::string("Optional[") +
            (text == nullptr ? "null" : text->utf8()) + "]");
    }

private:
    ::jxx::Ptr<Optional<T>> self_() {
        return ::jxx::CAST<Optional<T>>(this->thisPtr());
    }

    ::jxx::Ptr<T> value_;
};

} // namespace jxx::util
