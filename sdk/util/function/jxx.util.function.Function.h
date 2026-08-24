#pragma once

#include "lang/jxx.lang.Object.h"

namespace jxx {
namespace util {
namespace function {

template <typename T, typename R>
class Function {
public:
    virtual ~Function() = default;
    virtual jxx::Ptr<R> apply(const jxx::Ptr<T> t) = 0;

    template <typename V>
    jxx::Ptr<Function<V, R>> compose(const jxx::Ptr<Function<V, T>> before) {
        class ComposeFunction : public virtual Function<V, R> {
        private:
            jxx::Ptr<Function<V, T>> before_;
            jxx::Ptr<Function<T, R>> after_;
        public:
            ComposeFunction(const jxx::Ptr<Function<V, T>> before, jxx::Ptr<Function<T, R>> after)
                : before_(before), after_(after) {}
            
            virtual ~ComposeFunction() = default;
            
            virtual jxx::Ptr<R> apply(const jxx::Ptr<V> v) override {
                return after_->apply(before_->apply(v));
            }
        };
        
        return jxx::Ptr<Function<V, R>>(new ComposeFunction(before, jxx::Ptr<Function<T, R>>(this)));
    }

    template <typename V>
    jxx::Ptr<Function<T, V>> andThen(const jxx::Ptr<Function<R, V>> after) {
        class AndThenFunction : public virtual Function<T, V> {
        private:
            jxx::Ptr<Function<T, R>> before_;
            jxx::Ptr<Function<R, V>> after_;
        public:
            AndThenFunction(const jxx::Ptr<Function<T, R>> before, jxx::Ptr<Function<R, V>> after)
                : before_(before), after_(after) {}
            
            virtual ~AndThenFunction() = default;
            
            virtual jxx::Ptr<V> apply(const jxx::Ptr<T> t) override {
                return after_->apply(before_->apply(t));
            }
        };
        
        return jxx::Ptr<Function<T, V>>(new AndThenFunction(jxx::Ptr<Function<T, R>>(this), after));
    }

    static jxx::Ptr<Function<T, T>> identity() {
        class IdentityFunction : public virtual Function<T, T> {
        public:
            virtual ~IdentityFunction() = default;
            virtual jxx::Ptr<T> apply(const jxx::Ptr<T> t) override { return t; }
        };
        return jxx::Ptr<Function<T, T>>(new IdentityFunction());
    }
};

} // namespace function
} // namespace util
} // namespace jxx
