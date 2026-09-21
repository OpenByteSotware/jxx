#include "awt/geom/jxx.awt.geom.Point2D.h"
#include <cmath>
#include <cstring>
#include <sstream>
namespace jxx::awt::geom {
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
}
