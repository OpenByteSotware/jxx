#pragma once

#include <type_traits>
#include <utility>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::util::function {

    template <class T>
    class Consumer;

namespace detail {

template <class T>
class ComposedConsumer final : public jxx::lang::Object, public Consumer<T> {
public:
    ComposedConsumer(jxx::Ptr<Consumer<T>> first, jxx::Ptr<Consumer<T>> second)
        : first_(std::move(first)), second_(std::move(second)) {}

    void accept(T t) override {
        first_->accept(t);
        second_->accept(t);
    }

private:
    jxx::Ptr<Consumer<T>> first_;
    jxx::Ptr<Consumer<T>> second_;
};

template <class T, class U>
class ComposedConsumerCovariant final : public jxx::lang::Object, public Consumer<T> {
public:
    ComposedConsumerCovariant(jxx::Ptr<Consumer<T>> first, jxx::Ptr<Consumer<U>> second)
        : first_(std::move(first)), second_(std::move(second)) {}

    void accept(T t) override {
        first_->accept(t);
        // Java '? super T' means U can accept T (upcast). We model that with convertibility.
        second_->accept(static_cast<U>(t));
    }

private:
    jxx::Ptr<Consumer<T>> first_;
    jxx::Ptr<Consumer<U>> second_;
};

template <class T, class F>
class LambdaConsumer final : public jxx::lang::Object, public Consumer<T> {
public:
    explicit LambdaConsumer(F f) : f_(std::move(f)) {}
    void accept(T t) override { f_(t); }
private:
    F f_;
};

} // namespace detail

/**
 * Java 8 parity: java.util.function.Consumer<T>
 *
 */
template <class T>
struct Consumer {
    virtual ~Consumer() = default;

    // Java: void accept(T t)
    virtual void accept(T t) = 0;

    // ------------------------------------------------------------------
    // Default method: exact type
    // ------------------------------------------------------------------
    jxx::Ptr<Consumer<T>> andThen(jxx::Ptr<Consumer<T>> after) {
        if (!after) {
            throw jxx::lang::NullPointerException(JXX_NEW<std::string>("after"));
        }
        auto self = thisPtrAsConsumer_();
        return JXX_NEW<detail::ComposedConsumer<T>>(std::move(self), std::move(after));
    }

    // ------------------------------------------------------------------
    // Default method: variance-friendly
    //
    // Java: Consumer<? super T>
    // C++: allow Consumer<U> when T is convertible to U (U is “supertype”)
    // ------------------------------------------------------------------
    template <
        class U,
        typename std::enable_if_t<
            !std::is_same_v<T, U> &&
            (std::is_convertible_v<T, U> || std::is_constructible_v<U, T>),
            int
        > = 0
    >
    jxx::Ptr<Consumer<T>> andThen(jxx::Ptr<Consumer<U>> after) {
        if (!after) {
            throw jxx::lang::NullPointerException(JXX_NEW<std::string>("after"));
        }
        auto self = thisPtrAsConsumer_();
        return JXX_NEW<detail::ComposedConsumerCovariant<T, U>>(std::move(self), std::move(after));
    }

private:
    // Requires concrete implementer to derive from Object and be created via JXX_NEW.
    jxx::Ptr<Consumer<T>> thisPtrAsConsumer_() {
        auto* obj = dynamic_cast<jxx::lang::Object*>(this);
        if (!obj || !obj->thisPtr) {
            throw jxx::lang::IllegalArgumentException(
                JXX_NEW<std::string>(
                    "Consumer::andThen requires implementing object to derive from jxx::lang::Object "
                    "and be created via JXX_NEW so Object::thisPtr is set"
                )
            );
        }
        return std::static_pointer_cast<Consumer<T>>(obj->thisPtr);
    }
};

/**
 * Factory: build a Consumer<T> from a lambda/functor.
 * Returned object derives from Object so andThen() works.
 */
template <class T, class F>
jxx::Ptr<Consumer<T>> consumerOf(F&& f) {
    using Fn = std::decay_t<F>;
    return JXX_NEW<detail::LambdaConsumer<T, Fn>>(std::forward<F>(f));
}

} // namespace jxx::util::function