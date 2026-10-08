#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "util/function/jxx.util.function.PredicateSuper.h"

namespace jxx::util::function {

template <typename T>
class Predicate
    : public ::jxx::lang::InterfaceBase<Predicate<T>, PredicateSuper<T>> {
public:
    ~Predicate() override = default;

    ::jxx::Ptr<Predicate<T>> and_(
        const ::jxx::Ptr<PredicateSuper<T>>& other) {
        if (other == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        class AndPredicate final
            : public ::jxx::lang::ClassBase<
                  AndPredicate,
                  ::jxx::lang::Object,
                  Predicate<T>> {
        public:
            AndPredicate(
                const ::jxx::Ptr<PredicateSuper<T>>& first,
                const ::jxx::Ptr<PredicateSuper<T>>& second)
                : first_(first), second_(second) {
            }

            ::jxx::lang::jbool test(
                const ::jxx::Ptr<T>& value) override {
                return first_->test(value) && second_->test(value);
            }

        private:
            ::jxx::Ptr<PredicateSuper<T>> first_;
            ::jxx::Ptr<PredicateSuper<T>> second_;
        };

        return ::jxx::CAST<Predicate<T>>(
            ::jxx::NEW<AndPredicate>(selfSuper_(), other));
    }

    ::jxx::Ptr<Predicate<T>> negate() {
        class NegatePredicate final
            : public ::jxx::lang::ClassBase<
                  NegatePredicate,
                  ::jxx::lang::Object,
                  Predicate<T>> {
        public:
            explicit NegatePredicate(
                const ::jxx::Ptr<PredicateSuper<T>>& inner)
                : inner_(inner) {
            }

            ::jxx::lang::jbool test(
                const ::jxx::Ptr<T>& value) override {
                return !inner_->test(value);
            }

        private:
            ::jxx::Ptr<PredicateSuper<T>> inner_;
        };

        return ::jxx::CAST<Predicate<T>>(
            ::jxx::NEW<NegatePredicate>(selfSuper_()));
    }

    ::jxx::Ptr<Predicate<T>> or_(
        const ::jxx::Ptr<PredicateSuper<T>>& other) {
        if (other == nullptr) {
            throw ::jxx::lang::NullPointerException();
        }

        class OrPredicate final
            : public ::jxx::lang::ClassBase<
                  OrPredicate,
                  ::jxx::lang::Object,
                  Predicate<T>> {
        public:
            OrPredicate(
                const ::jxx::Ptr<PredicateSuper<T>>& first,
                const ::jxx::Ptr<PredicateSuper<T>>& second)
                : first_(first), second_(second) {
            }

            ::jxx::lang::jbool test(
                const ::jxx::Ptr<T>& value) override {
                return first_->test(value) || second_->test(value);
            }

        private:
            ::jxx::Ptr<PredicateSuper<T>> first_;
            ::jxx::Ptr<PredicateSuper<T>> second_;
        };

        return ::jxx::CAST<Predicate<T>>(
            ::jxx::NEW<OrPredicate>(selfSuper_(), other));
    }

    template <typename U>
    static ::jxx::Ptr<Predicate<U>> isEqual(
        const ::jxx::Ptr<::jxx::lang::Object>& targetReference) {
        class EqualityPredicate final
            : public ::jxx::lang::ClassBase<
                  EqualityPredicate,
                  ::jxx::lang::Object,
                  Predicate<U>> {
        public:
            explicit EqualityPredicate(
                const ::jxx::Ptr<::jxx::lang::Object>& target)
                : target_(target) {
            }

            ::jxx::lang::jbool test(
                const ::jxx::Ptr<U>& value) override {
                auto object = ::jxx::CAST<::jxx::lang::Object>(value);
                if (target_ == nullptr) {
                    return object == nullptr;
                }
                return target_->equals(object);
            }

        private:
            ::jxx::Ptr<::jxx::lang::Object> target_;
        };

        return ::jxx::CAST<Predicate<U>>(
            ::jxx::NEW<EqualityPredicate>(targetReference));
    }

private:
    ::jxx::Ptr<PredicateSuper<T>> selfSuper_() {
        auto* object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) {
            throw ::jxx::lang::IllegalStateException();
        }
        auto self = ::jxx::CAST<PredicateSuper<T>>(object->thisPtr());
        if (self == nullptr) {
            throw ::jxx::lang::IllegalStateException();
        }
        return self;
    }
};

} // namespace jxx::util::function
