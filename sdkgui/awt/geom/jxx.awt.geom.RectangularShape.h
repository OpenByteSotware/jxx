#pragma once

#include "awt/jxx.awt.Shape.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::geom
{
    class Dimension2D;

    class RectangularShape : public ::jxx::lang::ClassBase<
        RectangularShape, ::jxx::lang::Object, ::jxx::awt::Shape>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<RectangularShape,
            JxxSuper, ::jxx::awt::Shape>;

        virtual ::jxx::lang::jdouble getX() const = 0;
        virtual ::jxx::lang::jdouble getY() const = 0;
        virtual ::jxx::lang::jdouble getWidth() const = 0;
        virtual ::jxx::lang::jdouble getHeight() const = 0;
        virtual ::jxx::lang::jbool isEmpty() const = 0;
        virtual void setFrame(::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y, ::jxx::lang::jdouble width,
            ::jxx::lang::jdouble height) = 0;

        ::jxx::lang::jdouble getMinX() const;
        ::jxx::lang::jdouble getMinY() const;
        ::jxx::lang::jdouble getMaxX() const;
        ::jxx::lang::jdouble getMaxY() const;
        ::jxx::lang::jdouble getCenterX() const;
        ::jxx::lang::jdouble getCenterY() const;
        ::jxx::Ptr<Rectangle2D> getFrame() const;
        void setFrame(const ::jxx::Ptr<Point2D>& location,
            const ::jxx::Ptr<Dimension2D>& size);
        void setFrameFromDiagonal(::jxx::lang::jdouble x1,
            ::jxx::lang::jdouble y1, ::jxx::lang::jdouble x2,
            ::jxx::lang::jdouble y2);
        void setFrameFromCenter(::jxx::lang::jdouble centerX,
            ::jxx::lang::jdouble centerY, ::jxx::lang::jdouble cornerX,
            ::jxx::lang::jdouble cornerY);
        ::jxx::Ptr<::jxx::awt::Rectangle> getBounds() const override;
        virtual ::jxx::Ptr<PathIterator> getPathIterator(
            const ::jxx::Ptr<AffineTransform>& transform) const override = 0;
        ::jxx::Ptr<PathIterator> getPathIterator(
            const ::jxx::Ptr<AffineTransform>& transform,
            ::jxx::lang::jdouble flatness) const override;
    };
}
