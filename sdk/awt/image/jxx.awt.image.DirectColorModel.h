#pragma once

#include "awt/image/jxx.awt.image.PackedColorModel.h"

namespace jxx::awt::image {

class DirectColorModel
    : public ::jxx::lang::ClassBase<DirectColorModel, PackedColorModel> {
public:
    using JxxSuper = PackedColorModel;
    using Super = ::jxx::lang::ClassBase<DirectColorModel, JxxSuper>;

    DirectColorModel(::jxx::lang::jint bits, ::jxx::lang::jint redMask,
        ::jxx::lang::jint greenMask, ::jxx::lang::jint blueMask);
    DirectColorModel(::jxx::lang::jint bits, ::jxx::lang::jint redMask,
        ::jxx::lang::jint greenMask, ::jxx::lang::jint blueMask,
        ::jxx::lang::jint alphaMask);

    ::jxx::lang::jint getRed(::jxx::lang::jint pixel) const override;
    ::jxx::lang::jint getGreen(::jxx::lang::jint pixel) const override;
    ::jxx::lang::jint getBlue(::jxx::lang::jint pixel) const override;
    ::jxx::lang::jint getAlpha(::jxx::lang::jint pixel) const override;
};

} // namespace jxx::awt::image
