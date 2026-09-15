#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt { class Insets : public ::jxx::lang::ClassBase<Insets,::jxx::lang::Object>{ public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Insets,JxxSuper>; ::jxx::lang::jint top,left,bottom,right; Insets(::jxx::lang::jint top,::jxx::lang::jint left,::jxx::lang::jint bottom,::jxx::lang::jint right); void set(::jxx::lang::jint top,::jxx::lang::jint left,::jxx::lang::jint bottom,::jxx::lang::jint right); ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override; ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override; }; }
