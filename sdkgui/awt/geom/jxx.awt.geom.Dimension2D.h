#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::awt::geom {
class Dimension2D : public ::jxx::lang::ClassBase<Dimension2D,::jxx::lang::Object> {
public:
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Dimension2D,JxxSuper>;
 virtual ::jxx::lang::jdouble getWidth() const=0;
 virtual ::jxx::lang::jdouble getHeight() const=0;
 virtual void setSize(::jxx::lang::jdouble width,::jxx::lang::jdouble height)=0;
 void setSize(const ::jxx::Ptr<Dimension2D>& dimension);
}; }
