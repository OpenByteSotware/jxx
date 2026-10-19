#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include <cmath>
#include <cstring>
#include <sstream>
namespace jxx::awt::geom {
Point2D::Point2D() : Super() {}
Point2D::Float::Float() : Super() {}
Point2D::Float::Float(::jxx::lang::jfloat nx, ::jxx::lang::jfloat ny) : Super(), x(nx), y(ny) {}
Point2D::Double::Double() : Super() {}
Point2D::Double::Double(::jxx::lang::jdouble nx, ::jxx::lang::jdouble ny) : Super(), x(nx), y(ny) {}

void Point2D::setLocation(const ::jxx::Ptr<Point2D>& p){if(p)setLocation(p->getX(),p->getY());}
::jxx::lang::jdouble Point2D::distanceSq(::jxx::lang::jdouble x1,::jxx::lang::jdouble y1,::jxx::lang::jdouble x2,::jxx::lang::jdouble y2){const auto dx=x1-x2,dy=y1-y2;return dx*dx+dy*dy;}
::jxx::lang::jdouble Point2D::distance(::jxx::lang::jdouble x1,::jxx::lang::jdouble y1,::jxx::lang::jdouble x2,::jxx::lang::jdouble y2){return std::sqrt(distanceSq(x1,y1,x2,y2));}
::jxx::lang::jdouble Point2D::distanceSq(::jxx::lang::jdouble x,::jxx::lang::jdouble y) const{return distanceSq(getX(),getY(),x,y);}
::jxx::lang::jdouble Point2D::distanceSq(const ::jxx::Ptr<Point2D>& p) const{return p?distanceSq(p->getX(),p->getY()):0;}
::jxx::lang::jdouble Point2D::distance(::jxx::lang::jdouble x,::jxx::lang::jdouble y) const{return std::sqrt(distanceSq(x,y));}
::jxx::lang::jdouble Point2D::distance(const ::jxx::Ptr<Point2D>& p) const{return std::sqrt(distanceSq(p));}
::jxx::lang::jbool Point2D::equals(const ::jxx::Ptr<::jxx::lang::Object>& o) const{auto p=::jxx::CAST<Point2D>(o);return p&&getX()==p->getX()&&getY()==p->getY();}
::jxx::lang::jint Point2D::hashCode() const{std::uint64_t a=0,b=0;auto x=getX(),y=getY();std::memcpy(&a,&x,8);std::memcpy(&b,&y,8);auto bits=a^(a>>32)^b^(b>>32);return static_cast<::jxx::lang::jint>(bits);}
::jxx::Ptr<::jxx::lang::String> Point2D::Float::toString() const{std::ostringstream s;s<<"Point2D.Float["<<x<<", "<<y<<"]";return ::jxx::NEW<::jxx::lang::String>(s.str());}
::jxx::Ptr<::jxx::lang::String> Point2D::Double::toString() const{std::ostringstream s;s<<"Point2D.Double["<<x<<", "<<y<<"]";return ::jxx::NEW<::jxx::lang::String>(s.str());}

void Point2D::Float::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out){if(out){out->writeFloat(x);out->writeFloat(y);}}
void Point2D::Float::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in){if(in){x=in->readFloat();y=in->readFloat();}}
void Point2D::Float::readObjectNoData(){x=0;y=0;}
void Point2D::Double::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out){if(out){out->writeDouble(x);out->writeDouble(y);}}
void Point2D::Double::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in){if(in){x=in->readDouble();y=in->readDouble();}}
void Point2D::Double::readObjectNoData(){x=0;y=0;}
}
