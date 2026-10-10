#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"

namespace jxx::util::function {

class LongPredicate;

enum class LongPredicateOperation { And, Or, Negate };

class LongPredicateComposition;

#if 0
class LongPredicateComposition final
    : public ::jxx::lang::ClassBase<
          LongPredicateComposition,
          ::jxx::lang::Object,
          LongPredicate> {
public:
    LongPredicateComposition(
        const ::jxx::Ptr<LongPredicate>& first,
        const ::jxx::Ptr<LongPredicate>& second,
        LongPredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {
    }

    ::jxx::lang::jbool test(::jxx::lang::jlong value) override;

private:
    ::jxx::Ptr<LongPredicate> first_;
    ::jxx::Ptr<LongPredicate> second_;
    LongPredicateOperation operation_;
};
#endif

class LongPredicate : public ::jxx::lang::InterfaceBase<LongPredicate> {
public:
    ~LongPredicate() override = default;
    virtual ::jxx::lang::jbool test(::jxx::lang::jlong value) = 0;

    ::jxx::Ptr<LongPredicate> and_(const ::jxx::Ptr<LongPredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, LongPredicateOperation::And);
    }

    ::jxx::Ptr<LongPredicate> or_(const ::jxx::Ptr<LongPredicate>& other) {
        if (other == nullptr) throw ::jxx::lang::NullPointerException();
        return compose_(other, LongPredicateOperation::Or);
    }

    ::jxx::Ptr<LongPredicate> negate() {
        return compose_(nullptr, LongPredicateOperation::Negate);
    }

private:
    ::jxx::Ptr<LongPredicate> self_() {
        auto object = dynamic_cast<::jxx::lang::Object*>(this);
        if (object == nullptr) throw ::jxx::lang::IllegalStateException();
        auto self = std::dynamic_pointer_cast<LongPredicate>(object->thisPtr());
        if (self == nullptr) throw ::jxx::lang::IllegalStateException();
        return self;
    }

    ::jxx::Ptr<LongPredicate> compose_(
        const ::jxx::Ptr<LongPredicate>& other,
        LongPredicateOperation operation);
};

class LongPredicateComposition final
    : public ::jxx::lang::ClassBase<
          LongPredicateComposition,
          ::jxx::lang::Object,
          LongPredicate> {
public:
    LongPredicateComposition(const ::jxx::Ptr<LongPredicate>& first,
                        const ::jxx::Ptr<LongPredicate>& second,
                        LongPredicateOperation operation)
        : first_(first), second_(second), operation_(operation) {}
    ::jxx::lang::jbool test(::jxx::lang::jlong value) override;
private:
    ::jxx::Ptr<LongPredicate> first_;
    ::jxx::Ptr<LongPredicate> second_;
    LongPredicateOperation operation_;
};

inline ::jxx::Ptr<LongPredicate> LongPredicate::compose_(
    const ::jxx::Ptr<LongPredicate>& other,
    LongPredicateOperation operation) {
    auto result = ::jxx::NEW<LongPredicateComposition>(self_(), other, operation);
    return std::static_pointer_cast<LongPredicate>(result);
}

inline ::jxx::lang::jbool LongPredicateComposition::test(::jxx::lang::jlong value) {
    switch (operation_) {
    case LongPredicateOperation::And:
        return first_->test(value) && second_->test(value);
    case LongPredicateOperation::Or:
        return first_->test(value) || second_->test(value);
    case LongPredicateOperation::Negate:
        return !first_->test(value);
    }
    return false;
}

} // namespace jxx::util::function
