#pragma once

#include "awt/jxx.awt.Graphics.h"

namespace jxx::awt::font { class FontRenderContext; class GlyphVector; }
namespace jxx::awt::geom { class AffineTransform; }
namespace jxx::awt::image { class BufferedImage; class BufferedImageOp; class RenderedImage; }
namespace jxx::awt
{
    class Composite;
    class GraphicsConfiguration;
    class Paint;
    class RenderingHints;
    class Stroke;

    class Graphics2D : public ::jxx::lang::ClassBase<Graphics2D, Graphics>
    {
    public:
        using JxxSuper=Graphics;
        using Super=::jxx::lang::ClassBase<Graphics2D,JxxSuper>;
        ~Graphics2D() override=default;

        virtual void draw(const ::jxx::Ptr<Shape>& shape)=0;
        virtual void fill(const ::jxx::Ptr<Shape>& shape)=0;
        virtual void drawString(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jfloat x,::jxx::lang::jfloat y)=0;
        virtual void clip(const ::jxx::Ptr<Shape>& shape)=0;
        virtual ::jxx::lang::jbool hit(const ::jxx::Ptr<Rectangle>& rectangle,
            const ::jxx::Ptr<Shape>& shape,::jxx::lang::jbool onStroke)const=0;
        virtual ::jxx::Ptr<::jxx::awt::GraphicsConfiguration>
            getDeviceConfiguration()const=0;
        virtual void setComposite(const ::jxx::Ptr<Composite>& composite)=0;
        virtual void setPaint(const ::jxx::Ptr<Paint>& paint)=0;
        virtual void setStroke(const ::jxx::Ptr<Stroke>& stroke)=0;
        virtual void setRenderingHints(const ::jxx::Ptr<RenderingHints>& hints)=0;
        virtual ::jxx::Ptr<RenderingHints> getRenderingHints()const=0;
        virtual void rotate(::jxx::lang::jdouble theta)=0;
        virtual void rotate(::jxx::lang::jdouble theta,
            ::jxx::lang::jdouble x,::jxx::lang::jdouble y)=0;
        virtual void scale(::jxx::lang::jdouble sx,::jxx::lang::jdouble sy)=0;
        virtual void shear(::jxx::lang::jdouble shx,::jxx::lang::jdouble shy)=0;
        virtual void transform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform)=0;
        virtual void setTransform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>& transform)=0;
        virtual ::jxx::Ptr<::jxx::awt::geom::AffineTransform> getTransform()const=0;
        virtual ::jxx::Ptr<Paint> getPaint()const=0;
        virtual ::jxx::Ptr<Composite> getComposite()const=0;
        virtual void setBackground(const ::jxx::Ptr<Color>& color)=0;
        virtual ::jxx::Ptr<Color> getBackground()const=0;
        virtual ::jxx::Ptr<Stroke> getStroke()const=0;
        virtual ::jxx::Ptr<::jxx::awt::font::FontRenderContext>
            getFontRenderContext()const=0;

    protected:
        Graphics2D();
    };
}
