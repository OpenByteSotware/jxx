#include "awt/geom/jxx.awt.geom.AffineTransform.h"
#include "awt/geom/jxx.awt.geom.NoninvertibleTransformException.h"
#include "lang/jxx.lang.String.h"
#include <cmath>
#include "awt/geom/jxx.awt.geom.Point2D.h"

namespace jxx::awt::geom
{
    namespace
    {
        class TransformPoint final : public ::jxx::lang::ClassBase<
            TransformPoint, Point2D>
        {
        public:
            using JxxSuper = Point2D;
            using Super = ::jxx::lang::ClassBase<TransformPoint, JxxSuper>;

            TransformPoint() : Super() {}
            ::jxx::lang::jdouble getX() const override { return x_; }
            ::jxx::lang::jdouble getY() const override { return y_; }
            void setLocation(::jxx::lang::jdouble x,
                ::jxx::lang::jdouble y) override
            {
                x_ = x;
                y_ = y;
            }

        private:
            ::jxx::lang::jdouble x_ = 0.0;
            ::jxx::lang::jdouble y_ = 0.0;
        };
    }

    AffineTransform::AffineTransform() : Super() {}

    AffineTransform::AffineTransform(::jxx::lang::jdouble a,
        ::jxx::lang::jdouble b, ::jxx::lang::jdouble c,
        ::jxx::lang::jdouble d, ::jxx::lang::jdouble e,
        ::jxx::lang::jdouble f)
        : Super(), m00_(a), m10_(b), m01_(c), m11_(d), m02_(e), m12_(f)
    {
    }

    ::jxx::Ptr<AffineTransform> AffineTransform::getTranslateInstance(
        ::jxx::lang::jdouble x, ::jxx::lang::jdouble y)
    {
        return ::jxx::NEW<AffineTransform>(1, 0, 0, 1, x, y);
    }

    ::jxx::Ptr<AffineTransform> AffineTransform::getScaleInstance(
        ::jxx::lang::jdouble x, ::jxx::lang::jdouble y)
    {
        return ::jxx::NEW<AffineTransform>(x, 0, 0, y, 0, 0);
    }

    void AffineTransform::setToIdentity()
    {
        m00_ = m11_ = 1;
        m10_ = m01_ = m02_ = m12_ = 0;
    }

    ::jxx::lang::jbool AffineTransform::isIdentity() const
    {
        return m00_ == 1 && m11_ == 1 && m10_ == 0 && m01_ == 0 &&
            m02_ == 0 && m12_ == 0;
    }

    void AffineTransform::translate(::jxx::lang::jdouble x,
        ::jxx::lang::jdouble y)
    {
        m02_ += m00_ * x + m01_ * y;
        m12_ += m10_ * x + m11_ * y;
    }

    void AffineTransform::scale(::jxx::lang::jdouble x,
        ::jxx::lang::jdouble y)
    {
        m00_ *= x;
        m10_ *= x;
        m01_ *= y;
        m11_ *= y;
    }

    ::jxx::lang::jdouble AffineTransform::getDeterminant() const
    {
        return m00_ * m11_ - m01_ * m10_;
    }

    ::jxx::Ptr<Point2D> AffineTransform::transform(
        const ::jxx::Ptr<Point2D>& source,
        const ::jxx::Ptr<Point2D>& destination) const
    {
        if (source == nullptr)
            return nullptr;
        auto output = destination != nullptr ? destination :
            ::jxx::NEW<TransformPoint>();
        output->setLocation(
            m00_ * source->getX() + m01_ * source->getY() + m02_,
            m10_ * source->getX() + m11_ * source->getY() + m12_);
        return output;
    }

    ::jxx::Ptr<AffineTransform> AffineTransform::getRotateInstance(::jxx::lang::jdouble theta)
    { auto value=::jxx::NEW<AffineTransform>(); value->rotate(theta); return value; }
    ::jxx::Ptr<AffineTransform> AffineTransform::getRotateInstance(::jxx::lang::jdouble theta,::jxx::lang::jdouble ax,::jxx::lang::jdouble ay)
    { auto value=::jxx::NEW<AffineTransform>(); value->rotate(theta,ax,ay); return value; }
    void AffineTransform::concatenate(const ::jxx::Ptr<AffineTransform>& t)
    { if(!t)return; const auto a=m00_*t->m00_+m01_*t->m10_, b=m10_*t->m00_+m11_*t->m10_, c=m00_*t->m01_+m01_*t->m11_, d=m10_*t->m01_+m11_*t->m11_, e=m00_*t->m02_+m01_*t->m12_+m02_, f=m10_*t->m02_+m11_*t->m12_+m12_; m00_=a;m10_=b;m01_=c;m11_=d;m02_=e;m12_=f; }
    void AffineTransform::preConcatenate(const ::jxx::Ptr<AffineTransform>& t)
    { if(!t)return; const auto a=t->m00_*m00_+t->m01_*m10_, b=t->m10_*m00_+t->m11_*m10_, c=t->m00_*m01_+t->m01_*m11_, d=t->m10_*m01_+t->m11_*m11_, e=t->m00_*m02_+t->m01_*m12_+t->m02_, f=t->m10_*m02_+t->m11_*m12_+t->m12_; m00_=a;m10_=b;m01_=c;m11_=d;m02_=e;m12_=f; }
    void AffineTransform::rotate(::jxx::lang::jdouble theta)
    { const auto c=std::cos(theta),s=std::sin(theta); concatenate(::jxx::NEW<AffineTransform>(c,s,-s,c,0,0)); }
    void AffineTransform::rotate(::jxx::lang::jdouble theta,::jxx::lang::jdouble ax,::jxx::lang::jdouble ay)
    { translate(ax,ay); rotate(theta); translate(-ax,-ay); }
    void AffineTransform::shear(::jxx::lang::jdouble x,::jxx::lang::jdouble y)
    { concatenate(::jxx::NEW<AffineTransform>(1,y,x,1,0,0)); }
    ::jxx::Ptr<AffineTransform> AffineTransform::createInverse() const
    { auto value=::jxx::NEW<AffineTransform>(m00_,m10_,m01_,m11_,m02_,m12_); value->invert(); return value; }
    void AffineTransform::invert()
    { const auto det=getDeterminant(); if(std::abs(det)<=1.0e-15)throw NoninvertibleTransformException(::jxx::NEW<::jxx::lang::String>("Determinant is zero")); const auto a=m11_/det,b=-m10_/det,c=-m01_/det,d=m00_/det,e=(m01_*m12_-m11_*m02_)/det,f=(m10_*m02_-m00_*m12_)/det; m00_=a;m10_=b;m01_=c;m11_=d;m02_=e;m12_=f; }
    void AffineTransform::getMatrix(const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jdouble,1>>& matrix) const
    { if(!matrix)return; if(matrix->size()>0)(*matrix)[0]=m00_;if(matrix->size()>1)(*matrix)[1]=m10_;if(matrix->size()>2)(*matrix)[2]=m01_;if(matrix->size()>3)(*matrix)[3]=m11_;if(matrix->size()>4)(*matrix)[4]=m02_;if(matrix->size()>5)(*matrix)[5]=m12_; }
}
