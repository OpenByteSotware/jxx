#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/function/jxx.util.function.LongConsumer.h"
#include "util/function/jxx.util.function.LongSupplier.h"
#include "util/function/jxx.util.function.Supplier.h"
namespace jxx::util {
class OptionalLong final : public ::jxx::lang::ClassBase<OptionalLong,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<OptionalLong,JxxSuper>;
 static ::jxx::Ptr<OptionalLong> empty(); static ::jxx::Ptr<OptionalLong> of(::jxx::lang::jlong value);
 OptionalLong(); explicit OptionalLong(::jxx::lang::jlong value);
 ::jxx::lang::jlong getAsLong() const; ::jxx::lang::jbool isPresent() const noexcept;
 void ifPresent(const ::jxx::Ptr<::jxx::util::function::LongConsumer>& consumer) const;
 ::jxx::lang::jlong orElse(::jxx::lang::jlong other) const noexcept;
 ::jxx::lang::jlong orElseGet(const ::jxx::Ptr<::jxx::util::function::LongSupplier>& supplier) const;
 template<typename X> ::jxx::lang::jlong orElseThrow(const ::jxx::Ptr<::jxx::util::function::Supplier<X>>& supplier) const { if(present_) return value_; if(!supplier) throw ::jxx::lang::NullPointerException(); auto ex=supplier->get(); if(!ex) throw ::jxx::lang::NullPointerException(); throw *ex; }
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::lang::jbool present_=false; ::jxx::lang::jlong value_={};
}; }
