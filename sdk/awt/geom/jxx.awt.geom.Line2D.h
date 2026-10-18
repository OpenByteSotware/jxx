#pragma once

#include "awt/jxx.awt.Shape.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::geom
{
    class Line2D : public ::jxx::lang::ClassBase<Line2D,
        ::jxx::lang::Object, ::jxx::awt::Shape>
    {
    public:
        class Double;
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<Line2D,JxxSuper,::jxx::awt::Shape>;
        virtual ::jxx::lang::jdouble getX1()const=0;
        virtual ::jxx::lang::jdouble getY1()const=0;
        virtual ::jxx::lang::jdouble getX2()const=0;
        virtual ::jxx::lang::jdouble getY2()const=0;
        virtual void setLine(::jxx::lang::jdouble x1,::jxx::lang::jdouble y1,
            ::jxx::lang::jdouble x2,::jxx::lang::jdouble y2)=0;
        static ::jxx::lang::jdouble ptSegDistSq(::jxx::lang::jdouble x1,
            ::jxx::lang::jdouble y1,::jxx::lang::jdouble x2,
            ::jxx::lang::jdouble y2,::jxx::lang::jdouble px,
            ::jxx::lang::jdouble py);
        ::jxx::lang::jdouble ptSegDist(::jxx::lang::jdouble px,
            ::jxx::lang::jdouble py)const;
        ::jxx::Ptr<::jxx::awt::Rectangle> getBounds()const override;
        ::jxx::Ptr<Rectangle2D> getBounds2D()const override;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble,::jxx::lang::jdouble)const override{return false;}
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Point2D>&)const override{return false;}
        ::jxx::lang::jbool contains(::jxx::lang::jdouble,::jxx::lang::jdouble,::jxx::lang::jdouble,::jxx::lang::jdouble)const override{return false;}
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Rectangle2D>&)const override{return false;}
        ::jxx::lang::jbool intersects(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const override;
        ::jxx::lang::jbool intersects(const ::jxx::Ptr<Rectangle2D>& r)const override;
        ::jxx::Ptr<PathIterator> getPathIterator(const ::jxx::Ptr<AffineTransform>&)const override{return nullptr;}
        ::jxx::Ptr<PathIterator> getPathIterator(const ::jxx::Ptr<AffineTransform>&,::jxx::lang::jdouble)const override{return nullptr;}
    };
    class Line2D::Double final : public ::jxx::lang::ClassBase<Double,Line2D>
    { public: using JxxSuper=Line2D;using Super=::jxx::lang::ClassBase<Double,JxxSuper>; Double()=default;Double(::jxx::lang::jdouble a,::jxx::lang::jdouble b,::jxx::lang::jdouble c,::jxx::lang::jdouble d):x1(a),y1(b),x2(c),y2(d){} ::jxx::lang::jdouble x1=0,y1=0,x2=0,y2=0;::jxx::lang::jdouble getX1()const override{return x1;}::jxx::lang::jdouble getY1()const override{return y1;}::jxx::lang::jdouble getX2()const override{return x2;}::jxx::lang::jdouble getY2()const override{return y2;}void setLine(::jxx::lang::jdouble a,::jxx::lang::jdouble b,::jxx::lang::jdouble c,::jxx::lang::jdouble d)override{x1=a;y1=b;x2=c;y2=d;} };
}
