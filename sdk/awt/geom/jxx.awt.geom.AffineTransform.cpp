#include "awt/geom/jxx.awt.geom.AffineTransform.h"
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
}
