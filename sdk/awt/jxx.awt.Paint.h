#pragma once

#include "awt/jxx.awt.Transparency.h"

namespace jxx::awt::geom
{
    class AffineTransform;
    class Rectangle2D;
}

namespace jxx::awt::image
{
    class ColorModel;
}

namespace jxx::awt
{
    class PaintContext;
    class Rectangle;
    class RenderingHints;

    class Paint : public ::jxx::lang::InterfaceBase<Paint, Transparency>
    {
    public:
        ~Paint() override = default;
        virtual ::jxx::Ptr<PaintContext> createContext(
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& colorModel,
            const ::jxx::Ptr<Rectangle>& deviceBounds,
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& userBounds,
            const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform,
            const ::jxx::Ptr<RenderingHints>& hints) const = 0;
    };
}
