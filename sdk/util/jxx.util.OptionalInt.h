#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "util/jxx.util.NoSuchElementException.h"
#include "util/function/jxx.util.function.IntConsumer.h"
#include "util/function/jxx.util.function.IntSupplier.h"
#include "util/function/jxx.util.function.Supplier.h"
namespace jxx::util {
class OptionalInt final : public ::jxx::lang::ClassBase<OptionalInt,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<OptionalInt,JxxSuper>;
 static ::jxx::Ptr<OptionalInt> empty(); static ::jxx::Ptr<OptionalInt> of(::jxx::lang::jint value);
 OptionalInt(); explicit OptionalInt(::jxx::lang::jint value);
 ::jxx::lang::jint getAsInt() const; ::jxx::lang::jbool isPresent() const noexcept;
 void ifPresent(const ::jxx::Ptr<::jxx::util::function::IntConsumer>& consumer) const;
 ::jxx::lang::jint orElse(::jxx::lang::jint other) const noexcept;
 ::jxx::lang::jint orElseGet(const ::jxx::Ptr<::jxx::util::function::IntSupplier>& supplier) const;
 template<typename X> ::jxx::lang::jint orElseThrow(const ::jxx::Ptr<::jxx::util::function::Supplier<X>>& supplier) const { if(present_) return value_; if(!supplier) throw ::jxx::lang::NullPointerException(); auto ex=supplier->get(); if(!ex) throw ::jxx::lang::NullPointerException(); throw *ex; }
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
private: ::jxx::lang::jbool present_=false; ::jxx::lang::jint value_={};
}; }
