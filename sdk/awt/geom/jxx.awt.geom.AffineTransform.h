#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt::geom
{
    class Point2D;
    class AffineTransform : public ::jxx::lang::ClassBase<
        AffineTransform,::jxx::lang::Object>
    {
    public:
        using JxxSuper=::jxx::lang::Object;
        using Super=::jxx::lang::ClassBase<AffineTransform,JxxSuper>;
        AffineTransform();
        AffineTransform(::jxx::lang::jdouble m00,::jxx::lang::jdouble m10,
            ::jxx::lang::jdouble m01,::jxx::lang::jdouble m11,
            ::jxx::lang::jdouble m02,::jxx::lang::jdouble m12);
        static ::jxx::Ptr<AffineTransform> getTranslateInstance(
            ::jxx::lang::jdouble tx,::jxx::lang::jdouble ty);
        static ::jxx::Ptr<AffineTransform> getScaleInstance(
            ::jxx::lang::jdouble sx,::jxx::lang::jdouble sy);
        void setToIdentity();
        ::jxx::lang::jbool isIdentity()const;
        void translate(::jxx::lang::jdouble tx,::jxx::lang::jdouble ty);
        void scale(::jxx::lang::jdouble sx,::jxx::lang::jdouble sy);
        ::jxx::lang::jdouble getDeterminant()const;
        ::jxx::Ptr<Point2D> transform(const ::jxx::Ptr<Point2D>& source,
            const ::jxx::Ptr<Point2D>& destination)const;
    private:
        ::jxx::lang::jdouble m00_=1,m10_=0,m01_=0,m11_=1,m02_=0,m12_=0;
    };
}
