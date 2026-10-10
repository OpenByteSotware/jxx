#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class LongUnaryOperator;
enum class LongUnaryOperatorOperation { Compose, AndThen, Identity };
class LongUnaryOperator : public ::jxx::lang::InterfaceBase<LongUnaryOperator> {
public:
 ~LongUnaryOperator() override=default;
 virtual ::jxx::lang::jlong applyAsLong(::jxx::lang::jlong operand)=0;
 ::jxx::Ptr<LongUnaryOperator> compose(const ::jxx::Ptr<LongUnaryOperator>& before);
 ::jxx::Ptr<LongUnaryOperator> andThen(const ::jxx::Ptr<LongUnaryOperator>& after);
 static ::jxx::Ptr<LongUnaryOperator> identity();
private:
 ::jxx::Ptr<LongUnaryOperator> self_();
};
class LongUnaryOperatorComposition final : public ::jxx::lang::ClassBase<LongUnaryOperatorComposition,::jxx::lang::Object,LongUnaryOperator> {
public:
 LongUnaryOperatorComposition(const ::jxx::Ptr<LongUnaryOperator>& first,const ::jxx::Ptr<LongUnaryOperator>& second,LongUnaryOperatorOperation op):first_(first),second_(second),op_(op){}
 ::jxx::lang::jlong applyAsLong(::jxx::lang::jlong value) override {
  if(op_==LongUnaryOperatorOperation::Identity) return value;
  if(op_==LongUnaryOperatorOperation::Compose) return first_->applyAsLong(second_->applyAsLong(value));
  return second_->applyAsLong(first_->applyAsLong(value));
 }
private: ::jxx::Ptr<LongUnaryOperator> first_; ::jxx::Ptr<LongUnaryOperator> second_; LongUnaryOperatorOperation op_;
};
inline ::jxx::Ptr<LongUnaryOperator> LongUnaryOperator::self_() { auto object=dynamic_cast<::jxx::lang::Object*>(this); if(!object) throw ::jxx::lang::IllegalStateException(); auto self=std::dynamic_pointer_cast<LongUnaryOperator>(object->thisPtr()); if(!self) throw ::jxx::lang::IllegalStateException(); return self; }
inline ::jxx::Ptr<LongUnaryOperator> LongUnaryOperator::compose(const ::jxx::Ptr<LongUnaryOperator>& before) { if(!before) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<LongUnaryOperatorComposition>(self_(),before,LongUnaryOperatorOperation::Compose); return std::static_pointer_cast<LongUnaryOperator>(result); }
inline ::jxx::Ptr<LongUnaryOperator> LongUnaryOperator::andThen(const ::jxx::Ptr<LongUnaryOperator>& after) { if(!after) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<LongUnaryOperatorComposition>(self_(),after,LongUnaryOperatorOperation::AndThen); return std::static_pointer_cast<LongUnaryOperator>(result); }
inline ::jxx::Ptr<LongUnaryOperator> LongUnaryOperator::identity() { auto result=::jxx::NEW<LongUnaryOperatorComposition>(nullptr,nullptr,LongUnaryOperatorOperation::Identity); return std::static_pointer_cast<LongUnaryOperator>(result); }
} // namespace jxx::util::function
