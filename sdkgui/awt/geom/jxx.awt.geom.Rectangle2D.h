#pragma once

#include "awt/jxx.awt.Shape.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::geom
{
    class Rectangle2D : public ::jxx::lang::ClassBase<Rectangle2D,
        ::jxx::lang::Object,::jxx::awt::Shape>
    {
    public:
        class Double;
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<Rectangle2D,JxxSuper,::jxx::awt::Shape>;
        virtual ::jxx::lang::jdouble getX()const=0;
        virtual ::jxx::lang::jdouble getY()const=0;
        virtual ::jxx::lang::jdouble getWidth()const=0;
        virtual ::jxx::lang::jdouble getHeight()const=0;
        virtual void setRect(::jxx::lang::jdouble x,::jxx::lang::jdouble y,
            ::jxx::lang::jdouble width,::jxx::lang::jdouble height)=0;
        ::jxx::lang::jbool isEmpty()const;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y)const override;
        ::jxx::lang::jbool intersects(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const override;
        ::jxx::lang::jbool contains(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h)const override;
        ::jxx::Ptr<::jxx::awt::Rectangle> getBounds()const override;
        ::jxx::Ptr<Rectangle2D> getBounds2D()const override;
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Point2D>& p)const override;
        ::jxx::lang::jbool intersects(const ::jxx::Ptr<Rectangle2D>& r)const override;
        ::jxx::lang::jbool contains(const ::jxx::Ptr<Rectangle2D>& r)const override;
        ::jxx::Ptr<PathIterator> getPathIterator(const ::jxx::Ptr<AffineTransform>&)const override;
        ::jxx::Ptr<PathIterator> getPathIterator(const ::jxx::Ptr<AffineTransform>&,::jxx::lang::jdouble)const override;
    };
    class Rectangle2D::Double final : public ::jxx::lang::ClassBase<Double,Rectangle2D>
    { public: using JxxSuper=Rectangle2D;using Super=::jxx::lang::ClassBase<Double,JxxSuper>; Double()=default;Double(::jxx::lang::jdouble x,::jxx::lang::jdouble y,::jxx::lang::jdouble w,::jxx::lang::jdouble h):x(x),y(y),width(w),height(h){} ::jxx::lang::jdouble x=0,y=0,width=0,height=0;::jxx::lang::jdouble getX()const override{return x;}::jxx::lang::jdouble getY()const override{return y;}::jxx::lang::jdouble getWidth()const override{return width;}::jxx::lang::jdouble getHeight()const override{return height;}void setRect(::jxx::lang::jdouble a,::jxx::lang::jdouble b,::jxx::lang::jdouble c,::jxx::lang::jdouble d)override{x=a;y=b;width=c;height=d;} };
}
