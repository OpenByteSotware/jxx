#pragma once

#include "awt/geom/jxx.awt.geom.RectangularShape.h"

namespace jxx::awt::geom
{
    class Ellipse2D : public ::jxx::lang::ClassBase<Ellipse2D,RectangularShape>
    {
    public:
        class Double;
        using JxxSuper=RectangularShape;using Super=::jxx::lang::ClassBase<Ellipse2D,JxxSuper>;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y)const override;
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Point2D>& p)const override;
        ::jxx::lang::jbool intersects(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const override;
        ::jxx::lang::jbool intersects(const ::jxx::Ptr<Rectangle2D>& r)const override;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const override;
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Rectangle2D>& r)const override;
        ::jxx::Ptr<Rectangle2D> getBounds2D()const override;
        ::jxx::Ptr<PathIterator> getPathIterator(const ::jxx::Ptr<AffineTransform>&)const override{return nullptr;}
    };
    class Ellipse2D::Double final : public ::jxx::lang::ClassBase<Double,Ellipse2D>
    { public: using JxxSuper=Ellipse2D;using Super=::jxx::lang::ClassBase<Double,JxxSuper>; Double()=default;Double(::jxx::lang::jdouble a,::jxx::lang::jdouble b,::jxx::lang::jdouble c,::jxx::lang::jdouble d):x(a),y(b),width(c),height(d){} ::jxx::lang::jdouble x=0,y=0,width=0,height=0;::jxx::lang::jdouble getX()const override{return x;}::jxx::lang::jdouble getY()const override{return y;}::jxx::lang::jdouble getWidth()const override{return width;}::jxx::lang::jdouble getHeight()const override{return height;}::jxx::lang::jbool isEmpty()const override{return width<=0||height<=0;}void setFrame(::jxx::lang::jdouble a,::jxx::lang::jdouble b,::jxx::lang::jdouble c,::jxx::lang::jdouble d)override{x=a;y=b;width=c;height=d;} };
}
