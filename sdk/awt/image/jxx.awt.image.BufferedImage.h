#pragma once

#include <cstddef>
#include <cstdint>
#include <memory>
#include <vector>

#include "awt/jxx.awt.Image.h"
#include "lang/jxx.lang.JxxArray.h"

namespace jxx::awt { class Graphics2D; }

namespace jxx::awt::image {

class BufferedImage final
    : public ::jxx::lang::ClassBase<BufferedImage, ::jxx::awt::Image> {
public:
    using JxxSuper = ::jxx::awt::Image;
    using Super = ::jxx::lang::ClassBase<BufferedImage, JxxSuper>;

    static constexpr ::jxx::lang::jint TYPE_CUSTOM = 0;
    static constexpr ::jxx::lang::jint TYPE_INT_RGB = 1;
    static constexpr ::jxx::lang::jint TYPE_INT_ARGB = 2;

    BufferedImage(::jxx::lang::jint width, ::jxx::lang::jint height,
        ::jxx::lang::jint type);

    ::jxx::lang::jint getWidth(
        const ::jxx::Ptr<ImageObserver>& observer) const override;
    ::jxx::lang::jint getHeight(
        const ::jxx::Ptr<ImageObserver>& observer) const override;
    ::jxx::lang::jint getType() const;
    ::jxx::Ptr<ImageProducer> getSource() const override;
    ::jxx::Ptr<::jxx::awt::Graphics> getGraphics() override;
    ::jxx::Ptr<::jxx::awt::Graphics2D> createGraphics();
    ::jxx::Ptr<::jxx::lang::Object> getProperty(
        const ::jxx::Ptr<::jxx::lang::Object>& name,
        const ::jxx::Ptr<ImageObserver>& observer) override;
    ::jxx::lang::jint getRGB(::jxx::lang::jint x,
        ::jxx::lang::jint y) const;
    void setRGB(::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint rgb);
    ::jxx::Ptr<BufferedImage> getSubimage(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint width,
        ::jxx::lang::jint height) const;

    const std::vector<std::uint32_t>& pixels() const;
    std::vector<std::uint32_t>& pixels();

private:
    std::size_t pixelIndex_(::jxx::lang::jint x,
        ::jxx::lang::jint y) const;

    ::jxx::lang::jint width_;
    ::jxx::lang::jint height_;
    ::jxx::lang::jint type_;
    ::jxx::lang::jint scanlineStride_;
    ::jxx::lang::jint originX_ = 0;
    ::jxx::lang::jint originY_ = 0;
    std::shared_ptr<std::vector<std::uint32_t>> pixels_;
};

} // namespace jxx::awt::image
