#include "awt/image/jxx.awt.image.BufferedImage.h"

#include <memory>

#include "awt/image/internal/jxx.awt.image.internal.BufferedImageSource.h"
#include "awt/internal/jxx.awt.internal.BufferedImageGraphics2D.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

namespace jxx::awt::image {

BufferedImage::BufferedImage(::jxx::lang::jint width,
    ::jxx::lang::jint height, ::jxx::lang::jint type)
    : Super(), width_(width), height_(height), type_(type),
      scanlineStride_(width)
{
    if (width <= 0 || height <= 0)
        throw ::jxx::lang::IllegalArgumentException("width/height");
    if (type != TYPE_INT_RGB && type != TYPE_INT_ARGB)
        throw ::jxx::lang::IllegalArgumentException("image type");
    const auto initial = type == TYPE_INT_RGB ? 0xff000000U : 0U;
    pixels_ = std::make_shared<std::vector<std::uint32_t>>(
        static_cast<std::size_t>(width) * static_cast<std::size_t>(height),
        initial);
}

::jxx::lang::jint BufferedImage::getWidth(
    const ::jxx::Ptr<ImageObserver>&) const { return width_; }

::jxx::lang::jint BufferedImage::getHeight(
    const ::jxx::Ptr<ImageObserver>&) const { return height_; }

::jxx::lang::jint BufferedImage::getType() const { return type_; }

::jxx::Ptr<ImageProducer> BufferedImage::getSource() const
{
    return std::make_shared<
        ::jxx::awt::image::internal::BufferedImageSource>(
            ::jxx::CAST<BufferedImage>(thisPtr()));
}

::jxx::Ptr<::jxx::awt::Graphics> BufferedImage::getGraphics()
{
    return createGraphics();
}

::jxx::Ptr<::jxx::awt::Graphics2D> BufferedImage::createGraphics()
{
    return ::jxx::NEW<::jxx::awt::internal::BufferedImageGraphics2D>(
        ::jxx::CAST<BufferedImage>(thisPtr()));
}

::jxx::Ptr<::jxx::lang::Object> BufferedImage::getProperty(
    const ::jxx::Ptr<::jxx::lang::Object>&,
    const ::jxx::Ptr<ImageObserver>&)
{
    return nullptr;
}

std::size_t BufferedImage::pixelIndex_(::jxx::lang::jint x,
    ::jxx::lang::jint y) const
{
    if (x < 0 || y < 0 || x >= width_ || y >= height_)
        throw ::jxx::lang::IndexOutOfBoundsException();
    return static_cast<std::size_t>(originY_ + y) *
        static_cast<std::size_t>(scanlineStride_) +
        static_cast<std::size_t>(originX_ + x);
}

::jxx::lang::jint BufferedImage::getRGB(::jxx::lang::jint x,
    ::jxx::lang::jint y) const
{
    return static_cast<::jxx::lang::jint>((*pixels_)[pixelIndex_(x, y)]);
}

void BufferedImage::setRGB(::jxx::lang::jint x, ::jxx::lang::jint y,
    ::jxx::lang::jint rgb)
{
    auto value = static_cast<std::uint32_t>(rgb);
    if (type_ == TYPE_INT_RGB) value |= 0xff000000U;
    (*pixels_)[pixelIndex_(x, y)] = value;
}

::jxx::Ptr<BufferedImage> BufferedImage::getSubimage(
    ::jxx::lang::jint x, ::jxx::lang::jint y,
    ::jxx::lang::jint width, ::jxx::lang::jint height) const
{
    if (width <= 0 || height <= 0 || x < 0 || y < 0 ||
        x > width_ - width || y > height_ - height)
        throw ::jxx::lang::IndexOutOfBoundsException();
    auto result = ::jxx::NEW<BufferedImage>(width, height, type_);
    result->pixels_ = pixels_;
    result->scanlineStride_ = scanlineStride_;
    result->originX_ = originX_ + x;
    result->originY_ = originY_ + y;
    return result;
}

const std::vector<std::uint32_t>& BufferedImage::pixels() const
{
    return *pixels_;
}

std::vector<std::uint32_t>& BufferedImage::pixels()
{
    return *pixels_;
}

} // namespace jxx::awt::image
