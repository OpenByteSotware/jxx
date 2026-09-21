#include "awt/jxx.awt.GradientPaint.h"

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Transparency.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    namespace
    {
        class GradientPoint final : public ::jxx::lang::ClassBase<
            GradientPoint, ::jxx::awt::geom::Point2D>
        {
        public:
            using JxxSuper = ::jxx::awt::geom::Point2D;
            using Super = ::jxx::lang::ClassBase<GradientPoint, JxxSuper>;

            GradientPoint(::jxx::lang::jdouble x, ::jxx::lang::jdouble y)
                : Super(), x_(x), y_(y)
            {
            }

            ::jxx::lang::jdouble getX() const override { return x_; }
            ::jxx::lang::jdouble getY() const override { return y_; }
            void setLocation(::jxx::lang::jdouble x,
                ::jxx::lang::jdouble y) override
            {
                x_ = x;
                y_ = y;
            }

        private:
            ::jxx::lang::jdouble x_;
            ::jxx::lang::jdouble y_;
        };
    }
    GradientPaint::GradientPaint(::jxx::lang::jfloat x1,
        ::jxx::lang::jfloat y1, const ::jxx::Ptr<Color>& color1,
        ::jxx::lang::jfloat x2, ::jxx::lang::jfloat y2,
        const ::jxx::Ptr<Color>& color2)
        : GradientPaint(x1, y1, color1, x2, y2, color2, false)
    {
    }

    GradientPaint::GradientPaint(::jxx::lang::jfloat x1,
        ::jxx::lang::jfloat y1, const ::jxx::Ptr<Color>& color1,
        ::jxx::lang::jfloat x2, ::jxx::lang::jfloat y2,
        const ::jxx::Ptr<Color>& color2, ::jxx::lang::jbool cyclic)
        : GradientPaint(
            ::jxx::NEW<GradientPoint>(x1, y1), color1,
            ::jxx::NEW<GradientPoint>(x2, y2), color2,
            cyclic)
    {
    }

    GradientPaint::GradientPaint(
        const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point1,
        const ::jxx::Ptr<Color>& color1,
        const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point2,
        const ::jxx::Ptr<Color>& color2)
        : GradientPaint(point1, color1, point2, color2, false)
    {
    }

    GradientPaint::GradientPaint(
        const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point1,
        const ::jxx::Ptr<Color>& color1,
        const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point2,
        const ::jxx::Ptr<Color>& color2, ::jxx::lang::jbool cyclic)
        : Super(), point1_(point1), color1_(color1), point2_(point2),
          color2_(color2), cyclic_(cyclic)
    {
        if (point1_ == nullptr || point2_ == nullptr ||
            color1_ == nullptr || color2_ == nullptr)
            throw ::jxx::lang::IllegalArgumentException("null gradient value");
    }

    ::jxx::Ptr<::jxx::awt::geom::Point2D> GradientPaint::getPoint1() const
    { return ::jxx::NEW<GradientPoint>(point1_->getX(), point1_->getY()); }
    ::jxx::Ptr<Color> GradientPaint::getColor1() const{return color1_;}
    ::jxx::Ptr<::jxx::awt::geom::Point2D> GradientPaint::getPoint2() const
    { return ::jxx::NEW<GradientPoint>(point2_->getX(), point2_->getY()); }
    ::jxx::Ptr<Color> GradientPaint::getColor2() const{return color2_;}
    ::jxx::lang::jbool GradientPaint::isCyclic() const{return cyclic_;}
    ::jxx::lang::jint GradientPaint::getTransparency() const
    { return color1_->getAlpha()==255&&color2_->getAlpha()==255?Transparency::OPAQUE:Transparency::TRANSLUCENT; }
    ::jxx::Ptr<PaintContext> GradientPaint::createContext(
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<Rectangle>&,
        const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>&,
        const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&,
        const ::jxx::Ptr<RenderingHints>&) const
    { return nullptr; }
}
