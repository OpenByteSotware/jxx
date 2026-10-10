#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class IntUnaryOperator;
enum class IntUnaryOperatorOperation { Compose, AndThen, Identity };
class IntUnaryOperator : public ::jxx::lang::InterfaceBase<IntUnaryOperator> {
public:
 ~IntUnaryOperator() override=default;
 virtual ::jxx::lang::jint applyAsInt(::jxx::lang::jint operand)=0;
 ::jxx::Ptr<IntUnaryOperator> compose(const ::jxx::Ptr<IntUnaryOperator>& before);
 ::jxx::Ptr<IntUnaryOperator> andThen(const ::jxx::Ptr<IntUnaryOperator>& after);
 static ::jxx::Ptr<IntUnaryOperator> identity();
private:
 ::jxx::Ptr<IntUnaryOperator> self_();
};
class IntUnaryOperatorComposition final : public ::jxx::lang::ClassBase<IntUnaryOperatorComposition,::jxx::lang::Object,IntUnaryOperator> {
public:
 IntUnaryOperatorComposition(const ::jxx::Ptr<IntUnaryOperator>& first,const ::jxx::Ptr<IntUnaryOperator>& second,IntUnaryOperatorOperation op):first_(first),second_(second),op_(op){}
 ::jxx::lang::jint applyAsInt(::jxx::lang::jint value) override {
  if(op_==IntUnaryOperatorOperation::Identity) return value;
  if(op_==IntUnaryOperatorOperation::Compose) return first_->applyAsInt(second_->applyAsInt(value));
  return second_->applyAsInt(first_->applyAsInt(value));
 }
private: ::jxx::Ptr<IntUnaryOperator> first_; ::jxx::Ptr<IntUnaryOperator> second_; IntUnaryOperatorOperation op_;
};
inline ::jxx::Ptr<IntUnaryOperator> IntUnaryOperator::self_() { auto object=dynamic_cast<::jxx::lang::Object*>(this); if(!object) throw ::jxx::lang::IllegalStateException(); auto self=std::dynamic_pointer_cast<IntUnaryOperator>(object->thisPtr()); if(!self) throw ::jxx::lang::IllegalStateException(); return self; }
inline ::jxx::Ptr<IntUnaryOperator> IntUnaryOperator::compose(const ::jxx::Ptr<IntUnaryOperator>& before) { if(!before) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<IntUnaryOperatorComposition>(self_(),before,IntUnaryOperatorOperation::Compose); return std::static_pointer_cast<IntUnaryOperator>(result); }
inline ::jxx::Ptr<IntUnaryOperator> IntUnaryOperator::andThen(const ::jxx::Ptr<IntUnaryOperator>& after) { if(!after) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<IntUnaryOperatorComposition>(self_(),after,IntUnaryOperatorOperation::AndThen); return std::static_pointer_cast<IntUnaryOperator>(result); }
inline ::jxx::Ptr<IntUnaryOperator> IntUnaryOperator::identity() { auto result=::jxx::NEW<IntUnaryOperatorComposition>(nullptr,nullptr,IntUnaryOperatorOperation::Identity); return std::static_pointer_cast<IntUnaryOperator>(result); }
} // namespace jxx::util::function
