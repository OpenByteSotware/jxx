#include "awt/jxx.awt.Polygon.h"

#include <algorithm>
#include <cmath>

#include "awt/jxx.awt.Rectangle.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    Polygon::Polygon() : Super(){syncArrays();}
    Polygon::Polygon(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1>>& x,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1>>& y,
        ::jxx::lang::jint count) : Super()
    {
        if (!x || !y || count < 0 || count > x->length || count > y->length)
            throw ::jxx::lang::IllegalArgumentException("npoints");
        for (::jxx::lang::jint i=0;i<count;++i){xs_.push_back((*x)[i]);ys_.push_back((*y)[i]);}
        syncArrays();
    }
    void Polygon::syncArrays(){npoints=static_cast<::jxx::lang::jint>(xs_.size());xpoints=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint,1>>(npoints);ypoints=::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint,1>>(npoints);for(::jxx::lang::jint i=0;i<npoints;++i){(*xpoints)[i]=xs_[i];(*ypoints)[i]=ys_[i];}}
    void Polygon::reset(){xs_.clear();ys_.clear();syncArrays();}
    void Polygon::invalidate(){}
    void Polygon::translate(::jxx::lang::jint dx,::jxx::lang::jint dy){for(auto&v:xs_)v+=dx;for(auto&v:ys_)v+=dy;syncArrays();}
    void Polygon::addPoint(::jxx::lang::jint x,::jxx::lang::jint y){xs_.push_back(x);ys_.push_back(y);syncArrays();}
    ::jxx::Ptr<Rectangle> Polygon::getBounds() const{if(xs_.empty())return ::jxx::NEW<Rectangle>();auto [xmin,xmax]=std::minmax_element(xs_.begin(),xs_.end());auto [ymin,ymax]=std::minmax_element(ys_.begin(),ys_.end());return ::jxx::NEW<Rectangle>(*xmin,*ymin,*xmax-*xmin,*ymax-*ymin);}
    ::jxx::Ptr<::jxx::awt::geom::Rectangle2D> Polygon::getBounds2D() const{return nullptr;}
    ::jxx::lang::jbool Polygon::contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y) const{bool inside=false;for(std::size_t i=0,j=xs_.size()?xs_.size()-1:0;i<xs_.size();j=i++){const bool crossing=((ys_[i]>y)!=(ys_[j]>y))&&(x<(xs_[j]-xs_[i])*(y-ys_[i])/static_cast<double>(ys_[j]-ys_[i])+xs_[i]);if(crossing)inside=!inside;}return inside;}
    ::jxx::lang::jbool Polygon::contains(const ::jxx::Ptr<::jxx::awt::geom::Point2D>& p)const{return p&&contains(p->getX(),p->getY());}
    ::jxx::lang::jbool Polygon::intersects(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const{return contains(x,y)||contains(x+w,y)||contains(x,y+h)||contains(x+w,y+h);}
    ::jxx::lang::jbool Polygon::intersects(const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>&)const{return false;}
    ::jxx::lang::jbool Polygon::contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const{return contains(x,y)&&contains(x+w,y)&&contains(x,y+h)&&contains(x+w,y+h);}
    ::jxx::lang::jbool Polygon::contains(const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>&)const{return false;}
    ::jxx::Ptr<::jxx::awt::geom::PathIterator> Polygon::getPathIterator(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&)const{return nullptr;}
    ::jxx::Ptr<::jxx::awt::geom::PathIterator> Polygon::getPathIterator(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&,::jxx::lang::jdouble)const{return nullptr;}
    void Polygon::writeObject(const ::jxx::Ptr<::jxx::io::ObjectOutputStream>&){}
    void Polygon::readObject(const ::jxx::Ptr<::jxx::io::ObjectInputStream>&){reset();}
    void Polygon::readObjectNoData(){reset();}
}
