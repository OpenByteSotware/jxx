#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class IntPredicate;

enum class IntPredicateOperation { And, Or, Negate };

class IntPredicateComposition;

#if 0
class IntPredicateComposition final
    : public ::jxx::lang::ClassBase<
          IntPredicateComposition,
          ::jxx::lang::Object,
          IntPredicate> {
public:
    IntPredicateComposition(
        const ::jxx::Ptr<IntPredicate>& first,
        const ::jxx::Ptr<IntPredicate>& second,
        IntPredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {
    }

    ::jxx::lang::jbool test(::jxx::lang::jint value) override;

private:
    ::jxx::Ptr<IntPredicate> first_;
    ::jxx::Ptr<IntPredicate> second_;
    IntPredicateOperation operation_;
};
#endif

class IntPredicate : public ::jxx::lang::InterfaceBase<IntPredicate> {
public:
    ~IntPredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jint value) = 0;

    ::jxx::Ptr<IntPredicate> and_(const ::jxx::Ptr<IntPredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, IntPredicateOperation::And);
    }

    ::jxx::Ptr<IntPredicate> or_(const ::jxx::Ptr<IntPredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, IntPredicateOperation::Or);
    }

    ::jxx::Ptr<IntPredicate> negate() {
        return compose_(nullptr, IntPredicateOperation::Negate);
    }

private:
    ::jxx::Ptr<IntPredicate> self_() {
        auto object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) throw ::jxx::lang::IllegalStateException();
        auto self = std::dynamic_pointer_cast<IntPredicate>(object->thisPtr());
        if (self == nullptr) throw ::jxx::lang::IllegalStateException();
        return self;
    }

    ::jxx::Ptr<IntPredicate> compose_(
        const ::jxx::Ptr<IntPredicate>& other,
        IntPredicateOperation operation);
};

class IntPredicateComposition final
    : public ::jxx::lang::ClassBase<
          IntPredicateComposition,
          ::jxx::lang::Object,
          IntPredicate> {
public:
    IntPredicateComposition(const ::jxx::Ptr<IntPredicate>& first,
                        const ::jxx::Ptr<IntPredicate>& second,
                        IntPredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {}
    ::jxx::lang::jbool test(::jxx::lang::jint value) override;
private:
    ::jxx::Ptr<IntPredicate> first_;
    ::jxx::Ptr<IntPredicate> second_;
    IntPredicateOperation operation_;
};

inline ::jxx::Ptr<IntPredicate> IntPredicate::compose_(
    const ::jxx::Ptr<IntPredicate>& other,
    IntPredicateOperation operation) {
    auto result = ::jxx::NEW<IntPredicateComposition>(self_(), other, operation);
    return std::static_pointer_cast<IntPredicate>(result);
}

inline ::jxx::lang::jbool IntPredicateComposition::test(::jxx::lang::jint value) {
    switch (operation_) {
    case IntPredicateOperation::And:
        return first_->test(value) && second_->test(value);
    case IntPredicateOperation::Or:
        return first_->test(value) || second_->test(value);
    case IntPredicateOperation::Negate:
        return !first_->test(value);
    }
    return false;
}

} // namespace jxx::util::function
