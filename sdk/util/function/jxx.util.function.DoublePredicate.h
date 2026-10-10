#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class DoublePredicate;

enum class DoublePredicateOperation { And, Or, Negate };

class DoublePredicateComposition;

#if 0
class DoublePredicateComposition final
    : public ::jxx::lang::ClassBase<
          DoublePredicateComposition,
          ::jxx::lang::Object,
          DoublePredicate> {
public:
    DoublePredicateComposition(
        const ::jxx::Ptr<DoublePredicate>& first,
        const ::jxx::Ptr<DoublePredicate>& second,
        DoublePredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {
    }

    ::jxx::lang::jbool test(::jxx::lang::jdouble value) override;

private:
    ::jxx::Ptr<DoublePredicate> first_;
    ::jxx::Ptr<DoublePredicate> second_;
    DoublePredicateOperation operation_;
};
#endif

class DoublePredicate : public ::jxx::lang::InterfaceBase<DoublePredicate> {
public:
    ~DoublePredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jdouble value) = 0;

    ::jxx::Ptr<DoublePredicate> and_(const ::jxx::Ptr<DoublePredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, DoublePredicateOperation::And);
    }

    ::jxx::Ptr<DoublePredicate> or_(const ::jxx::Ptr<DoublePredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, DoublePredicateOperation::Or);
    }

    ::jxx::Ptr<DoublePredicate> negate() {
        return compose_(nullptr, DoublePredicateOperation::Negate);
    }

private:
    ::jxx::Ptr<DoublePredicate> self_() {
        auto object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) throw ::jxx::lang::IllegalStateException();
        auto self = std::dynamic_pointer_cast<DoublePredicate>(object->thisPtr());
        if (self == nullptr) throw ::jxx::lang::IllegalStateException();
        return self;
    }

    ::jxx::Ptr<DoublePredicate> compose_(
        const ::jxx::Ptr<DoublePredicate>& other,
        DoublePredicateOperation operation);
};

class DoublePredicateComposition final
    : public ::jxx::lang::ClassBase<
          DoublePredicateComposition,
          ::jxx::lang::Object,
          DoublePredicate> {
public:
    DoublePredicateComposition(const ::jxx::Ptr<DoublePredicate>& first,
                        const ::jxx::Ptr<DoublePredicate>& second,
                        DoublePredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {}
    ::jxx::lang::jbool test(::jxx::lang::jdouble value) override;
private:
    ::jxx::Ptr<DoublePredicate> first_;
    ::jxx::Ptr<DoublePredicate> second_;
    DoublePredicateOperation operation_;
};

inline ::jxx::Ptr<DoublePredicate> DoublePredicate::compose_(
    const ::jxx::Ptr<DoublePredicate>& other,
    DoublePredicateOperation operation) {
    auto result = ::jxx::NEW<DoublePredicateComposition>(self_(), other, operation);
    return std::static_pointer_cast<DoublePredicate>(result);
}

inline ::jxx::lang::jbool DoublePredicateComposition::test(::jxx::lang::jdouble value) {
    switch (operation_) {
    case DoublePredicateOperation::And:
        return first_->test(value) && second_->test(value);
    case DoublePredicateOperation::Or:
        return first_->test(value) || second_->test(value);
    case DoublePredicateOperation::Negate:
        return !first_->test(value);
    }
    return false;
}

} // namespace jxx::util::function
