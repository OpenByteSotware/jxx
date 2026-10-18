#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt {
class Point : public ::jxx::lang::ClassBase<Point, ::jxx::lang::Object> {
public:
 using JxxSuper = ::jxx::lang::Object;
 using Super = ::jxx::lang::ClassBase<Point, JxxSuper>;
 ::jxx::lang::jint x = 0;
 ::jxx::lang::jint y = 0;
 Point() = default;
 Point(::jxx::lang::jint x, ::jxx::lang::jint y);
 Point(const ::jxx::Ptr<Point>& point);
 ::jxx::Ptr<Point> getLocation() const;
 void setLocation(const ::jxx::Ptr<Point>& point);
 void setLocation(::jxx::lang::jint x, ::jxx::lang::jint y);
 void move(::jxx::lang::jint x, ::jxx::lang::jint y);
 void translate(::jxx::lang::jint dx, ::jxx::lang::jint dy);
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override;
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
};
}
