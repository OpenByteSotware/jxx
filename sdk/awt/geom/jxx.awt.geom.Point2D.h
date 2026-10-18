#pragma once
#include "io/jxx.io.SerializableI.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx_types.h"
namespace jxx::awt::geom {
class Point2D : public ::jxx::lang::ClassBase<Point2D,::jxx::lang::Object> {
public:
 class Float; class Double;
 using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<Point2D,JxxSuper>;
 virtual ::jxx::lang::jdouble getX() const=0; virtual ::jxx::lang::jdouble getY() const=0;
 virtual void setLocation(::jxx::lang::jdouble x,::jxx::lang::jdouble y)=0;
 void setLocation(const ::jxx::Ptr<Point2D>& point);
 static ::jxx::lang::jdouble distanceSq(::jxx::lang::jdouble x1,::jxx::lang::jdouble y1,::jxx::lang::jdouble x2,::jxx::lang::jdouble y2);
 static ::jxx::lang::jdouble distance(::jxx::lang::jdouble x1,::jxx::lang::jdouble y1,::jxx::lang::jdouble x2,::jxx::lang::jdouble y2);
 ::jxx::lang::jdouble distanceSq(::jxx::lang::jdouble px,::jxx::lang::jdouble py) const;
 ::jxx::lang::jdouble distanceSq(const ::jxx::Ptr<Point2D>& point) const;
 ::jxx::lang::jdouble distance(::jxx::lang::jdouble px,::jxx::lang::jdouble py) const;
 ::jxx::lang::jdouble distance(const ::jxx::Ptr<Point2D>& point) const;
 ::jxx::lang::jbool equals(const ::jxx::Ptr<::jxx::lang::Object>& object) const override;
 ::jxx::lang::jint hashCode() const override;
};
class Point2D::Float : public ::jxx::lang::ClassBase<Point2D::Float,Point2D,::jxx::io::SerializableI> {
public:
 using JxxSuper=Point2D; using Super=::jxx::lang::ClassBase<Point2D::Float,JxxSuper,::jxx::io::SerializableI>;
 ::jxx::lang::jfloat x=0, y=0; Float()=default; Float(::jxx::lang::jfloat x,::jxx::lang::jfloat y):x(x),y(y){}
 ::jxx::lang::jdouble getX() const override{return x;} ::jxx::lang::jdouble getY() const override{return y;}
 void setLocation(::jxx::lang::jdouble nx,::jxx::lang::jdouble ny) override{x=static_cast<::jxx::lang::jfloat>(nx);y=static_cast<::jxx::lang::jfloat>(ny);}
 void setLocation(::jxx::lang::jfloat nx,::jxx::lang::jfloat ny){x=nx;y=ny;}
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
};
class Point2D::Double : public ::jxx::lang::ClassBase<Point2D::Double,Point2D,::jxx::io::SerializableI> {
public:
 using JxxSuper=Point2D; using Super=::jxx::lang::ClassBase<Point2D::Double,JxxSuper,::jxx::io::SerializableI>;
 ::jxx::lang::jdouble x=0, y=0; Double()=default; Double(::jxx::lang::jdouble x,::jxx::lang::jdouble y):x(x),y(y){}
 ::jxx::lang::jdouble getX() const override{return x;} ::jxx::lang::jdouble getY() const override{return y;}
 void setLocation(::jxx::lang::jdouble nx,::jxx::lang::jdouble ny) override{x=nx;y=ny;}
 ::jxx::Ptr<::jxx::lang::String> toString() const override;
}; }
