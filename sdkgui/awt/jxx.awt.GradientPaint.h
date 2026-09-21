#pragma once

#include "awt/jxx.awt.Paint.h"
#include "awt/geom/jxx.awt.geom.Point2D.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt
{
    class Color;

    class GradientPaint : public ::jxx::lang::ClassBase<GradientPaint,
        ::jxx::lang::Object, Paint>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<GradientPaint, JxxSuper, Paint>;

        GradientPaint(::jxx::lang::jfloat x1, ::jxx::lang::jfloat y1,
            const ::jxx::Ptr<Color>& color1,
            ::jxx::lang::jfloat x2, ::jxx::lang::jfloat y2,
            const ::jxx::Ptr<Color>& color2);
        GradientPaint(::jxx::lang::jfloat x1, ::jxx::lang::jfloat y1,
            const ::jxx::Ptr<Color>& color1,
            ::jxx::lang::jfloat x2, ::jxx::lang::jfloat y2,
            const ::jxx::Ptr<Color>& color2,
            ::jxx::lang::jbool cyclic);
        GradientPaint(const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point1,
            const ::jxx::Ptr<Color>& color1,
            const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point2,
            const ::jxx::Ptr<Color>& color2);
        GradientPaint(const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point1,
            const ::jxx::Ptr<Color>& color1,
            const ::jxx::Ptr<::jxx::awt::geom::Point2D>& point2,
            const ::jxx::Ptr<Color>& color2,
            ::jxx::lang::jbool cyclic);

        ::jxx::Ptr<::jxx::awt::geom::Point2D> getPoint1() const;
        ::jxx::Ptr<Color> getColor1() const;
        ::jxx::Ptr<::jxx::awt::geom::Point2D> getPoint2() const;
        ::jxx::Ptr<Color> getColor2() const;
        ::jxx::lang::jbool isCyclic() const;
        ::jxx::lang::jint getTransparency() const override;
        ::jxx::Ptr<PaintContext> createContext(
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& colorModel,
            const ::jxx::Ptr<Rectangle>& deviceBounds,
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& userBounds,
            const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform,
            const ::jxx::Ptr<RenderingHints>& hints) const override;

    private:
        ::jxx::Ptr<::jxx::awt::geom::Point2D> point1_;
        ::jxx::Ptr<Color> color1_;
        ::jxx::Ptr<::jxx::awt::geom::Point2D> point2_;
        ::jxx::Ptr<Color> color2_;
        ::jxx::lang::jbool cyclic_;
    };
}
