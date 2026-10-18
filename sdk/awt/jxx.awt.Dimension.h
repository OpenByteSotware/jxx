#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class Dimension : public ::jxx::lang::ClassBase<Dimension, ::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Dimension,JxxSuper>;
 ::jxx::lang::jint width=0; ::jxx::lang::jint height=0;
 Dimension()=default; Dimension(::jxx::lang::jint width, ::jxx::lang::jint height); Dimension(const ::jxx::Ptr<Dimension>& dimension);
 ::jxx::Ptr<Dimension> getSize() const; void setSize(const ::jxx::Ptr<Dimension>& dimension); void setSize(::jxx::lang::jint width, ::jxx::lang::jint height);
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override; ::jxx::lang::jint hashCode() const override; ::jxx::Ptr<::jxx::lang::String> toString() const override;
}; }
