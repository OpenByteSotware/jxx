#pragma once

#include "io/jxx.util.function.PredicateSuper.h"
#include "io/jxx.lang.Exceptions.h"

namespace jxx {
template <typename T> class Ptr;
}

namespace jxx {
namespace util {
namespace function {

template <typename T>
class Predicate : public virtual PredicateSuper<T> {
public:
    virtual ~Predicate() = default;

    virtual jxx::Ptr<Predicate<T>> and_(jxx::Ptr<PredicateSuper<T>> other) {
        if (other == nullptr) {
            throw NullPointerException();
        }

        class AndPredicate : public virtual Predicate<T> {
        private:
            jxx::Ptr<PredicateSuper<T>> a_;
            jxx::Ptr<PredicateSuper<T>> b_;

        public:
            AndPredicate(jxx::Ptr<PredicateSuper<T>> a, jxx::Ptr<PredicateSuper<T>> b)
                : a_(a), b_(b) {}

            virtual ~AndPredicate() = default;

            virtual jbool test(jxx::Ptr<T> value) override {
                return a_->test(value) && b_->test(value);
            }
        };

        return jxx::Ptr<Predicate<T>>(new AndPredicate(jxx::Ptr<PredicateSuper<T>>(this), other));
    }

    virtual jxx::Ptr<Predicate<T>> negate() {
        class NegatePredicate : public virtual Predicate<T> {
        private:
            jxx::Ptr<PredicateSuper<T>> inner_;

        public:
            explicit NegatePredicate(jxx::Ptr<PredicateSuper<T>> inner)
                : inner_(inner) {}

            virtual ~NegatePredicate() = default;

            virtual jbool test(jxx::Ptr<T> value) override {
                return !inner_->test(value);
            }
        };

        return jxx::Ptr<Predicate<T>>(new NegatePredicate(jxx::Ptr<PredicateSuper<T>>(this)));
    }

    virtual jxx::Ptr<Predicate<T>> or_(jxx::Ptr<PredicateSuper<T>> other) {
        if (other == nullptr) {
            throw NullPointerException();
        }

        class OrPredicate : public virtual Predicate<T> {
        private:
            jxx::Ptr<PredicateSuper<T>> a_;
            jxx::Ptr<PredicateSuper<T>> b_;

        public:
            OrPredicate(jxx::Ptr<PredicateSuper<T>> a, jxx::Ptr<PredicateSuper<T>> b)
                : a_(a), b_(b) {}

            virtual ~OrPredicate() = default;

            virtual jbool test(jxx::Ptr<T> value) override {
                return a_->test(value) || b_->test(value);
            }
        };

        return jxx::Ptr<Predicate<T>>(new OrPredicate(jxx::Ptr<PredicateSuper<T>>(this), other));
    }
};

} // namespace function
} // namespace util
} // namespace jxx
