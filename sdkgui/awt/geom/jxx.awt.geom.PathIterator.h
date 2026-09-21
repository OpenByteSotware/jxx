#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx_types.h"
#include "lang/jxx.lang.JxxArray.h"
namespace jxx::awt::geom {
class PathIterator : public ::jxx::lang::InterfaceBase<PathIterator> {
public:
 static constexpr ::jxx::lang::jint WIND_EVEN_ODD=0, WIND_NON_ZERO=1;
 static constexpr ::jxx::lang::jint SEG_MOVETO=0, SEG_LINETO=1, SEG_QUADTO=2, SEG_CUBICTO=3, SEG_CLOSE=4;
 ~PathIterator() override=default;
 virtual ::jxx::lang::jint getWindingRule() const=0;
 virtual ::jxx::lang::jbool isDone() const=0;
 virtual void next()=0;
 virtual ::jxx::lang::jint currentSegment(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jfloat>>& coords) const=0;
 virtual ::jxx::lang::jint currentSegment(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jdouble>>& coords) const=0;
}; }
