#pragma once

#include "awt/jxx.awt.Paint.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"

namespace jxx::awt::image
{
    class BufferedImage;
}

namespace jxx::awt
{
    class TexturePaint : public ::jxx::lang::ClassBase<TexturePaint,
        ::jxx::lang::Object, Paint>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<TexturePaint, JxxSuper, Paint>;

        TexturePaint(
            const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image,
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& anchor);
        ::jxx::Ptr<::jxx::awt::image::BufferedImage> getImage() const;
        ::jxx::Ptr<::jxx::awt::geom::Rectangle2D> getAnchorRect() const;
        ::jxx::lang::jint getTransparency() const override;
        ::jxx::Ptr<PaintContext> createContext(
            const ::jxx::Ptr<::jxx::awt::image::ColorModel>& colorModel,
            const ::jxx::Ptr<Rectangle>& deviceBounds,
            const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& userBounds,
            const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform,
            const ::jxx::Ptr<RenderingHints>& hints) const override;

    private:
        ::jxx::Ptr<::jxx::awt::image::BufferedImage> image_;
        ::jxx::Ptr<::jxx::awt::geom::Rectangle2D> anchor_;
    };
}
