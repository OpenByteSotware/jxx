#include "awt/image/jxx.awt.image.DirectColorModel.h"

namespace jxx::awt::image {

DirectColorModel::DirectColorModel(
    ::jxx::lang::jint bits, ::jxx::lang::jint redMask,
    ::jxx::lang::jint greenMask, ::jxx::lang::jint blueMask)
    : DirectColorModel(bits, redMask, greenMask, blueMask, 0) {
}

DirectColorModel::DirectColorModel(
    ::jxx::lang::jint bits, ::jxx::lang::jint redMask,
    ::jxx::lang::jint greenMask, ::jxx::lang::jint blueMask,
    ::jxx::lang::jint alphaMask)
    : Super(bits, redMask, greenMask, blueMask, alphaMask) {
}

::jxx::lang::jint DirectColorModel::getRed(::jxx::lang::jint pixel) const {
    return extractComponent(pixel, redMask_);
}
::jxx::lang::jint DirectColorModel::getGreen(::jxx::lang::jint pixel) const {
    return extractComponent(pixel, greenMask_);
}
::jxx::lang::jint DirectColorModel::getBlue(::jxx::lang::jint pixel) const {
    return extractComponent(pixel, blueMask_);
}
::jxx::lang::jint DirectColorModel::getAlpha(::jxx::lang::jint pixel) const {
    return alphaMask_ == 0 ? 255 : extractComponent(pixel, alphaMask_);
}

} // namespace jxx::awt::image
