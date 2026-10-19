#include "awt/internal/jxx.awt.internal.BufferedImageGraphics2D.h"

#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Rectangle.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace jxx::awt::internal
{
    BufferedImageGraphics2D::BufferedImageGraphics2D(
        const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image)
        : Graphics2D(), image_(image), color_(Color::black),
          background_(Color::black)
    {
        if (image_ != nullptr)
            clip_ = ::jxx::NEW<Rectangle>(0, 0,
                image_->getWidth(nullptr), image_->getHeight(nullptr));
    }

    ::jxx::Ptr<Graphics> BufferedImageGraphics2D::create() const
    {
        auto result = ::jxx::NEW<BufferedImageGraphics2D>(image_);
        result->color_ = color_;
        result->background_ = background_;
        result->font_ = font_;
        result->translateX_ = translateX_;
        result->translateY_ = translateY_;
        if (clip_ != nullptr)
            result->clip_ = ::jxx::NEW<Rectangle>(clip_->x, clip_->y,
                clip_->width, clip_->height);
        return result;
    }

    void BufferedImageGraphics2D::translate(::jxx::lang::jint x,
        ::jxx::lang::jint y)
    {
        translateX_ += x;
        translateY_ += y;
    }

    ::jxx::Ptr<Color> BufferedImageGraphics2D::getColor() const { return color_; }
    void BufferedImageGraphics2D::setColor(const ::jxx::Ptr<Color>& value)
    { if (value != nullptr) color_ = value; }
    void BufferedImageGraphics2D::setPaintMode() {}
    void BufferedImageGraphics2D::setXORMode(const ::jxx::Ptr<Color>&) {}
    ::jxx::Ptr<Font> BufferedImageGraphics2D::getFont() const { return font_; }
    void BufferedImageGraphics2D::setFont(const ::jxx::Ptr<Font>& value) { font_ = value; }
    ::jxx::Ptr<FontMetrics> BufferedImageGraphics2D::getFontMetrics(const ::jxx::Ptr<Font>&) const { return nullptr; }
    ::jxx::Ptr<Rectangle> BufferedImageGraphics2D::getClipBounds() const
    { return clip_ == nullptr ? nullptr : ::jxx::NEW<Rectangle>(clip_->x, clip_->y, clip_->width, clip_->height); }

    void BufferedImageGraphics2D::clipRect(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        auto requested = ::jxx::NEW<Rectangle>(x + translateX_, y + translateY_, width, height);
        clip_ = clip_ == nullptr ? requested : clip_->intersection(requested);
    }

    void BufferedImageGraphics2D::setClip(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    { clip_ = ::jxx::NEW<Rectangle>(x + translateX_, y + translateY_, width, height); }
    ::jxx::Ptr<Shape> BufferedImageGraphics2D::getClip() const { return nullptr; }
    void BufferedImageGraphics2D::setClip(const ::jxx::Ptr<Shape>&) {}

    auto BufferedImageGraphics2D::insideClip(::jxx::lang::jint x,
        ::jxx::lang::jint y) const -> ::jxx::lang::jbool
    {
        return image_ != nullptr && x >= 0 && y >= 0 &&
            x < image_->getWidth(nullptr) && y < image_->getHeight(nullptr) &&
            (clip_ == nullptr || clip_->contains(x, y));
    }

    void BufferedImageGraphics2D::putPixel(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint argb)
    { if (insideClip(x, y)) image_->setRGB(x, y, argb); }

    void BufferedImageGraphics2D::copyArea(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height, ::jxx::lang::jint dx,
        ::jxx::lang::jint dy)
    {
        if (image_ == nullptr || width <= 0 || height <= 0) return;
        std::vector<::jxx::lang::jint> copy(static_cast<std::size_t>(width) * height);
        for (::jxx::lang::jint row = 0; row < height; ++row)
            for (::jxx::lang::jint column = 0; column < width; ++column)
            {
                const auto sx = x + translateX_ + column;
                const auto sy = y + translateY_ + row;
                copy[static_cast<std::size_t>(row) * width + column] =
                    insideClip(sx, sy) ? image_->getRGB(sx, sy) : 0;
            }
        for (::jxx::lang::jint row = 0; row < height; ++row)
            for (::jxx::lang::jint column = 0; column < width; ++column)
                putPixel(x + translateX_ + dx + column,
                    y + translateY_ + dy + row,
                    copy[static_cast<std::size_t>(row) * width + column]);
    }

    void BufferedImageGraphics2D::drawLine(::jxx::lang::jint x1,
        ::jxx::lang::jint y1, ::jxx::lang::jint x2,
        ::jxx::lang::jint y2)
    {
        x1 += translateX_; y1 += translateY_;
        x2 += translateX_; y2 += translateY_;
        const auto dx = std::abs(x2 - x1);
        const auto sx = x1 < x2 ? 1 : -1;
        const auto dy = -std::abs(y2 - y1);
        const auto sy = y1 < y2 ? 1 : -1;
        auto error = dx + dy;
        while (true)
        {
            putPixel(x1, y1, color_->getRGB());
            if (x1 == x2 && y1 == y2) break;
            const auto twice = 2 * error;
            if (twice >= dy) { error += dy; x1 += sx; }
            if (twice <= dx) { error += dx; y1 += sy; }
        }
    }

    void BufferedImageGraphics2D::fillRect(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        for (::jxx::lang::jint row = 0; row < height; ++row)
            for (::jxx::lang::jint column = 0; column < width; ++column)
                putPixel(x + translateX_ + column, y + translateY_ + row,
                    color_->getRGB());
    }

    void BufferedImageGraphics2D::clearRect(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        const auto previous = color_; color_ = background_;
        fillRect(x, y, width, height); color_ = previous;
    }

    void BufferedImageGraphics2D::drawOval(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        if (width <= 0 || height <= 0) return;
        constexpr double pi = 3.14159265358979323846;
        const auto steps = std::max(width, height) * 4;
        for (int index = 0; index < steps; ++index)
        {
            const auto angle = 2.0 * pi * index / steps;
            putPixel(x + translateX_ + static_cast<int>((width - 1) * (1.0 + std::cos(angle)) / 2.0),
                y + translateY_ + static_cast<int>((height - 1) * (1.0 + std::sin(angle)) / 2.0), color_->getRGB());
        }
    }

    void BufferedImageGraphics2D::fillOval(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height)
    {
        if (width <= 0 || height <= 0) return;
        const double rx = width / 2.0, ry = height / 2.0;
        for (int row = 0; row < height; ++row)
            for (int column = 0; column < width; ++column)
            {
                const double nx = (column + 0.5 - rx) / rx;
                const double ny = (row + 0.5 - ry) / ry;
                if (nx * nx + ny * ny <= 1.0)
                    putPixel(x + translateX_ + column, y + translateY_ + row, color_->getRGB());
            }
    }

    void BufferedImageGraphics2D::drawPolygon(const IntArray& xPoints,
        const IntArray& yPoints, ::jxx::lang::jint nPoints)
    {
        if (!xPoints || !yPoints || nPoints < 2) return;
        for (int index = 0; index < nPoints; ++index)
        {
            const int next = (index + 1) % nPoints;
            drawLine((*xPoints)[index], (*yPoints)[index],
                (*xPoints)[next], (*yPoints)[next]);
        }
    }

    void BufferedImageGraphics2D::fillPolygon(const IntArray& xPoints,
        const IntArray& yPoints, ::jxx::lang::jint nPoints)
    { drawPolygon(xPoints, yPoints, nPoints); }
    void BufferedImageGraphics2D::drawString(const ::jxx::Ptr<::jxx::lang::String>&, int, int) {}
    void BufferedImageGraphics2D::dispose() { image_ = nullptr; }

    ::jxx::lang::jbool BufferedImageGraphics2D::drawImage(
        const ::jxx::Ptr<Image>& source, ::jxx::lang::jint x,
        ::jxx::lang::jint y,
        const ::jxx::Ptr<::jxx::awt::image::ImageObserver>&)
    {
        auto image = ::jxx::CAST<::jxx::awt::image::BufferedImage>(source);
        if (!image || !image_) return false;
        for (int row = 0; row < image->getHeight(nullptr); ++row)
            for (int column = 0; column < image->getWidth(nullptr); ++column)
                putPixel(x + translateX_ + column, y + translateY_ + row,
                    image->getRGB(column, row));
        return true;
    }

    void BufferedImageGraphics2D::draw(const ::jxx::Ptr<Shape>&) {}
    void BufferedImageGraphics2D::fill(const ::jxx::Ptr<Shape>&) {}
    void BufferedImageGraphics2D::drawString(const ::jxx::Ptr<::jxx::lang::String>& text, float x, float y)
    { drawString(text, static_cast<int>(x), static_cast<int>(y)); }
    void BufferedImageGraphics2D::clip(const ::jxx::Ptr<Shape>&) {}
    bool BufferedImageGraphics2D::hit(const ::jxx::Ptr<Rectangle>&, const ::jxx::Ptr<Shape>&, bool) const { return false; }
    ::jxx::Ptr<GraphicsConfiguration> BufferedImageGraphics2D::getDeviceConfiguration() const { return nullptr; }
    void BufferedImageGraphics2D::setComposite(const ::jxx::Ptr<Composite>&) {}
    void BufferedImageGraphics2D::setPaint(const ::jxx::Ptr<Paint>&) {}
    void BufferedImageGraphics2D::setStroke(const ::jxx::Ptr<Stroke>&) {}
    void BufferedImageGraphics2D::setRenderingHints(const ::jxx::Ptr<RenderingHints>&) {}
    ::jxx::Ptr<RenderingHints> BufferedImageGraphics2D::getRenderingHints() const { return nullptr; }
    void BufferedImageGraphics2D::rotate(double) {}
    void BufferedImageGraphics2D::rotate(double, double, double) {}
    void BufferedImageGraphics2D::scale(double, double) {}
    void BufferedImageGraphics2D::shear(double, double) {}
    void BufferedImageGraphics2D::transform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&) {}
    void BufferedImageGraphics2D::setTransform(const ::jxx::Ptr<::jxx::awt::geom::AffineTransform>&) {}
    ::jxx::Ptr<::jxx::awt::geom::AffineTransform> BufferedImageGraphics2D::getTransform() const { return nullptr; }
    ::jxx::Ptr<Paint> BufferedImageGraphics2D::getPaint() const { return nullptr; }
    ::jxx::Ptr<Composite> BufferedImageGraphics2D::getComposite() const { return nullptr; }
    void BufferedImageGraphics2D::setBackground(const ::jxx::Ptr<Color>& value)
    { if (value != nullptr) background_ = value; }
    ::jxx::Ptr<Color> BufferedImageGraphics2D::getBackground() const { return background_; }
    ::jxx::Ptr<Stroke> BufferedImageGraphics2D::getStroke() const { return nullptr; }
    ::jxx::Ptr<::jxx::awt::font::FontRenderContext> BufferedImageGraphics2D::getFontRenderContext() const { return nullptr; }
}
