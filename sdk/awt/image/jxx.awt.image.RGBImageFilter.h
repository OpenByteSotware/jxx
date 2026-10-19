#pragma once

#include "awt/image/jxx.awt.image.ImageFilter.h"

namespace jxx::awt::image {

class RGBImageFilter
    : public ::jxx::lang::ClassBase<RGBImageFilter, ImageFilter> {
public:
    using JxxSuper = ImageFilter;
    using Super = ::jxx::lang::ClassBase<RGBImageFilter, JxxSuper>;

    RGBImageFilter();
    virtual ::jxx::lang::jint filterRGB(::jxx::lang::jint x,
        ::jxx::lang::jint y, ::jxx::lang::jint rgb) = 0;
    ::jxx::Ptr<ImageFilter> getFilterInstance(
        const ::jxx::Ptr<ImageConsumer>& consumer) override;
};

class RGBImageFilterInstance final : public ImageFilter {
public:
    RGBImageFilterInstance(const ::jxx::Ptr<RGBImageFilter>& filter,
        const ::jxx::Ptr<ImageConsumer>& consumer);

    void setPixels(::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint width, ::jxx::lang::jint height,
        const ::jxx::Ptr<ColorModel>& model,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1>>& pixels,
        ::jxx::lang::jint offset, ::jxx::lang::jint scansize) override;
    void setPixels(::jxx::lang::jint x, ::jxx::lang::jint y,
        ::jxx::lang::jint width, ::jxx::lang::jint height,
        const ::jxx::Ptr<ColorModel>& model,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>& pixels,
        ::jxx::lang::jint offset, ::jxx::lang::jint scansize) override;

private:
    ::jxx::Ptr<RGBImageFilter> filter_;
};

} // namespace jxx::awt::image
