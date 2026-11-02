#pragma once

#include "awt/image/jxx.awt.image.ColorModel.h"

namespace jxx::awt::image {

class PackedColorModel
    : public ::jxx::lang::ClassBase<PackedColorModel, ColorModel> {
public:
    using JxxSuper = ColorModel;
    using Super = ::jxx::lang::ClassBase<PackedColorModel, JxxSuper>;

    PackedColorModel(
        ::jxx::lang::jint bits,
        ::jxx::lang::jint redMask,
        ::jxx::lang::jint greenMask,
        ::jxx::lang::jint blueMask,
        ::jxx::lang::jint alphaMask);

    ::jxx::lang::jint getMask(::jxx::lang::jint index) const;
    ::jxx::lang::jint getRedMask() const;
    ::jxx::lang::jint getGreenMask() const;
    ::jxx::lang::jint getBlueMask() const;
    ::jxx::lang::jint getAlphaMask() const;
    ::jxx::lang::jint getTransparency() const override;

protected:
    static ::jxx::lang::jint extractComponent(
        ::jxx::lang::jint pixel,
        ::jxx::lang::jint mask);

    ::jxx::lang::jint redMask_;
    ::jxx::lang::jint greenMask_;
    ::jxx::lang::jint blueMask_;
    ::jxx::lang::jint alphaMask_;
};

} // namespace jxx::awt::image
