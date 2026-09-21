#include "awt/jxx.awt.TexturePaint.h"

#include "awt/jxx.awt.Transparency.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::awt
{
    TexturePaint::TexturePaint(
        const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image,
        const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>& anchor)
        : Super(), image_(image), anchor_(anchor)
    {
        if (image_ == nullptr || anchor_ == nullptr)
            throw ::jxx::lang::IllegalArgumentException("null texture value");
    }
    ::jxx::Ptr<::jxx::awt::image::BufferedImage> TexturePaint::getImage() const{return image_;}
    ::jxx::Ptr<::jxx::awt::geom::Rectangle2D> TexturePaint::getAnchorRect() const{return anchor_;}
    ::jxx::lang::jint TexturePaint::getTransparency() const{return Transparency::TRANSLUCENT;}
    ::jxx::Ptr<PaintContext> TexturePaint::createContext(
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<Rectangle>&,
        const ::jxx::Ptr<::jxx::awt::geom::Rectangle2D>&,
        const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&,
        const ::jxx::Ptr<RenderingHints>&) const{return nullptr;}
}
