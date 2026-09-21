#pragma once

#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace jxx::awt
{
    class Color;
    class Font;
    class FontMetrics;
    class Image;
    class Rectangle;
    class Shape;

    class Graphics : public ::jxx::lang::ClassBase<Graphics,
        ::jxx::lang::Object>
    {
    public:
        using JxxSuper = ::jxx::lang::Object;
        using Super = ::jxx::lang::ClassBase<Graphics, JxxSuper>;
        using IntArray = ::jxx::Ptr<::jxx::lang::JxxArray<
            ::jxx::lang::jint, 1>>;

        ~Graphics() override = default;
        virtual ::jxx::Ptr<Graphics> create() const = 0;
        virtual ::jxx::Ptr<Graphics> create(::jxx::lang::jint x,
            ::jxx::lang::jint y, ::jxx::lang::jint width,
            ::jxx::lang::jint height) const;
        virtual void translate(::jxx::lang::jint x,
            ::jxx::lang::jint y) = 0;
        virtual ::jxx::Ptr<Color> getColor() const = 0;
        virtual void setColor(const ::jxx::Ptr<Color>& color) = 0;
        virtual void setPaintMode() = 0;
        virtual void setXORMode(const ::jxx::Ptr<Color>& color) = 0;
        virtual ::jxx::Ptr<Font> getFont() const = 0;
        virtual void setFont(const ::jxx::Ptr<Font>& font) = 0;
        virtual ::jxx::Ptr<FontMetrics> getFontMetrics(
            const ::jxx::Ptr<Font>& font) const = 0;
        virtual ::jxx::Ptr<Rectangle> getClipBounds() const = 0;
        virtual void clipRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual void setClip(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual ::jxx::Ptr<Shape> getClip() const = 0;
        virtual void setClip(const ::jxx::Ptr<Shape>& clip) = 0;
        virtual void copyArea(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height,
            ::jxx::lang::jint dx, ::jxx::lang::jint dy) = 0;
        virtual void drawLine(::jxx::lang::jint x1, ::jxx::lang::jint y1,
            ::jxx::lang::jint x2, ::jxx::lang::jint y2) = 0;
        virtual void fillRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual void clearRect(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual void drawOval(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual void fillOval(::jxx::lang::jint x, ::jxx::lang::jint y,
            ::jxx::lang::jint width, ::jxx::lang::jint height) = 0;
        virtual void drawPolygon(const IntArray& xPoints,
            const IntArray& yPoints, ::jxx::lang::jint nPoints) = 0;
        virtual void fillPolygon(const IntArray& xPoints,
            const IntArray& yPoints, ::jxx::lang::jint nPoints) = 0;
        virtual void drawString(const ::jxx::Ptr<::jxx::lang::String>& text,
            ::jxx::lang::jint x, ::jxx::lang::jint y) = 0;
        virtual void dispose() = 0;
        ::jxx::Ptr<::jxx::lang::String> toString() const override;

    protected:
        Graphics();
    };
}
