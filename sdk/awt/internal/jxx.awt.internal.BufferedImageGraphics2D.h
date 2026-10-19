#pragma once

#include "awt/jxx.awt.Graphics2D.h"

namespace jxx::awt::image { class BufferedImage; }

namespace jxx::awt::internal
{
    class BufferedImageGraphics2D final : public ::jxx::awt::Graphics2D
    {
    public:
        explicit BufferedImageGraphics2D(
            const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image);

        ::jxx::Ptr<Graphics> create() const override;
        void translate(::jxx::lang::jint x, ::jxx::lang::jint y) override;
        ::jxx::Ptr<Color> getColor() const override;
        void setColor(const ::jxx::Ptr<Color>& color) override;
        void setPaintMode() override;
        void setXORMode(const ::jxx::Ptr<Color>& color) override;
        ::jxx::Ptr<Font> getFont() const override;
        void setFont(const ::jxx::Ptr<Font>& font) override;
        ::jxx::Ptr<FontMetrics> getFontMetrics(const ::jxx::Ptr<Font>& font) const override;
        ::jxx::Ptr<Rectangle> getClipBounds() const override;
        void clipRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void setClip(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        ::jxx::Ptr<Shape> getClip() const override;
        void setClip(const ::jxx::Ptr<Shape>& clip) override;
        void copyArea(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height,
            ::jxx::lang::jint dx, ::jxx::lang::jint dy) override;
        void drawLine(::jxx::lang::jint x1, ::jxx::lang::jint y1,
            ::jxx::lang::jint x2, ::jxx::lang::jint y2) override;
        void fillRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void clearRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void drawOval(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void fillOval(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) override;
        void drawPolygon(const IntArray& xPoints, const IntArray& yPoints,
            ::jxx::lang::jint nPoints) override;
        void fillPolygon(const IntArray& xPoints, const IntArray& yPoints,
            ::jxx::lang::jint nPoints) override;
        void drawString(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint x, ::jxx::lang::jint y) override;
        void dispose() override;
        ::jxx::lang::jbool drawImage(const ::jxx::Ptr<Image>& image,
            ::jxx::lang::jint x, ::jxx::lang::jint y,
            const ::jxx::Ptr<::jxx::awt::image::ImageObserver>& observer) override;

        void draw(const ::jxx::Ptr<Shape>& shape) override;
        void fill(const ::jxx::Ptr<Shape>& shape) override;
        void drawString(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jfloat x, ::jxx::lang::jfloat y) override;
        void clip(const ::jxx::Ptr<Shape>& shape) override;
        ::jxx::lang::jbool hit(const ::jxx::Ptr<Rectangle>& rectangle,
            const ::jxx::Ptr<Shape>& shape, ::jxx::lang::jbool onStroke) const override;
        ::jxx::Ptr<GraphicsConfiguration> getDeviceConfiguration() const override;
        void setComposite(const ::jxx::Ptr<Composite>& composite) override;
        void setPaint(const ::jxx::Ptr<Paint>& paint) override;
        void setStroke(const ::jxx::Ptr<Stroke>& stroke) override;
        void setRenderingHints(const ::jxx::Ptr<RenderingHints>& hints) override;
        ::jxx::Ptr<RenderingHints> getRenderingHints() const override;
        void rotate(::jxx::lang::jdouble theta) override;
        void rotate(::jxx::lang::jdouble theta, ::jxx::lang::jdouble x,
            ::jxx::lang::jdouble y) override;
        void scale(::jxx::lang::jdouble sx, ::jxx::lang::jdouble sy) override;
        void shear(::jxx::lang::jdouble shx, ::jxx::lang::jdouble shy) override;
        void transform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform) override;
        void setTransform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform) override;
        ::jxx::Ptr<::jxx::awt::geom::AffineTransform> getTransform() const override;
        ::jxx::Ptr<Paint> getPaint() const override;
        ::jxx::Ptr<Composite> getComposite() const override;
        void setBackground(const ::jxx::Ptr<Color>& color) override;
        ::jxx::Ptr<Color> getBackground() const override;
        ::jxx::Ptr<Stroke> getStroke() const override;
        ::jxx::Ptr<::jxx::awt::font::FontRenderContext> getFontRenderContext() const override;

    private:
        auto insideClip(::jxx::lang::jint x, ::jxx::lang::jint y) const
            -> ::jxx::lang::jbool;
        void putPixel(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint argb);

        ::jxx::Ptr<::jxx::awt::image::BufferedImage> image_;
        ::jxx::Ptr<Color> color_;
        ::jxx::Ptr<Color> background_;
        ::jxx::Ptr<Composite> composite_;
        ::jxx::Ptr<Font> font_;
        ::jxx::Ptr<Rectangle> clip_;
        ::jxx::lang::jint translateX_ = 0;
        ::jxx::lang::jint translateY_ = 0;
    };
}
