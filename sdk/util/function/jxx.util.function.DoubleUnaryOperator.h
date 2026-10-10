#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::util::function {
class DoubleUnaryOperator;
enum class DoubleUnaryOperatorOperation { Compose, AndThen, Identity };
class DoubleUnaryOperator : public ::jxx::lang::InterfaceBase<DoubleUnaryOperator> {
public:
 ~DoubleUnaryOperator() override=default;
 virtual ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jdouble operand)=0;
 ::jxx::Ptr<DoubleUnaryOperator> compose(const ::jxx::Ptr<DoubleUnaryOperator>& before);
 ::jxx::Ptr<DoubleUnaryOperator> andThen(const ::jxx::Ptr<DoubleUnaryOperator>& after);
 static ::jxx::Ptr<DoubleUnaryOperator> identity();
private:
 ::jxx::Ptr<DoubleUnaryOperator> self_();
};
class DoubleUnaryOperatorComposition final : public ::jxx::lang::ClassBase<DoubleUnaryOperatorComposition,::jxx::lang::Object,DoubleUnaryOperator> {
public:
 DoubleUnaryOperatorComposition(const ::jxx::Ptr<DoubleUnaryOperator>& first,const ::jxx::Ptr<DoubleUnaryOperator>& second,DoubleUnaryOperatorOperation op):first_(first),second_(second),op_(op){}
 ::jxx::lang::jdouble applyAsDouble(::jxx::lang::jdouble value) override {
  if(op_==DoubleUnaryOperatorOperation::Identity) return value;
  if(op_==DoubleUnaryOperatorOperation::Compose) return first_->applyAsDouble(second_->applyAsDouble(value));
  return second_->applyAsDouble(first_->applyAsDouble(value));
 }
private: ::jxx::Ptr<DoubleUnaryOperator> first_; ::jxx::Ptr<DoubleUnaryOperator> second_; DoubleUnaryOperatorOperation op_;
};
inline ::jxx::Ptr<DoubleUnaryOperator> DoubleUnaryOperator::self_() { auto object=dynamic_cast<::jxx::lang::Object*>(this); if(!object) throw ::jxx::lang::IllegalStateException(); auto self=std::dynamic_pointer_cast<DoubleUnaryOperator>(object->thisPtr()); if(!self) throw ::jxx::lang::IllegalStateException(); return self; }
inline ::jxx::Ptr<DoubleUnaryOperator> DoubleUnaryOperator::compose(const ::jxx::Ptr<DoubleUnaryOperator>& before) { if(!before) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<DoubleUnaryOperatorComposition>(self_(),before,DoubleUnaryOperatorOperation::Compose); return std::static_pointer_cast<DoubleUnaryOperator>(result); }
inline ::jxx::Ptr<DoubleUnaryOperator> DoubleUnaryOperator::andThen(const ::jxx::Ptr<DoubleUnaryOperator>& after) { if(!after) throw ::jxx::lang::NullPointerException(); auto result=::jxx::NEW<DoubleUnaryOperatorComposition>(self_(),after,DoubleUnaryOperatorOperation::AndThen); return std::static_pointer_cast<DoubleUnaryOperator>(result); }
inline ::jxx::Ptr<DoubleUnaryOperator> DoubleUnaryOperator::identity() { auto result=::jxx::NEW<DoubleUnaryOperatorComposition>(nullptr,nullptr,DoubleUnaryOperatorOperation::Identity); return std::static_pointer_cast<DoubleUnaryOperator>(result); }
} // namespace jxx::util::function
