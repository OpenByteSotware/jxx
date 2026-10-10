#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/function/jxx.util.function.DoubleConsumer.h"
#include "util/function/jxx.util.function.DoubleSupplier.h"
#include "util/function/jxx.util.function.Supplier.h"
namespace jxx::util {
class OptionalDouble final : public ::jxx::lang::ClassBase<OptionalDouble,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<OptionalDouble,JxxSuper>;
 static ::jxx::Ptr<OptionalDouble> empty(); static ::jxx::Ptr<OptionalDouble> of(::jxx::lang::jdouble value);
 OptionalDouble(); explicit OptionalDouble(::jxx::lang::jdouble value);
 ::jxx::lang::jdouble getAsDouble() const; ::jxx::lang::jbool isPresent() const noexcept;
 void ifPresent(const ::jxx::Ptr<::jxx::util::function::DoubleConsumer>& consumer) const;
 ::jxx::lang::jdouble orElse(::jxx::lang::jdouble other) const noexcept;
 ::jxx::lang::jdouble orElseGet(const ::jxx::Ptr<::jxx::util::function::DoubleSupplier>& supplier) const;
 template<typename X> ::jxx::lang::jdouble orElseThrow(const ::jxx::Ptr<::jxx::util::function::Supplier<X>>& supplier) const { if(present_) return value_; if(!supplier) throw ::jxx::lang::NullPointerException(); auto ex=supplier->get(); if(!ex) throw ::jxx::lang::NullPointerException(); throw *ex; }
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::lang::jbool present_=false; ::jxx::lang::jdouble value_={};
}; }
