#include "awt/image/jxx.awt.image.PackedColorModel.h"

#include "lang/jxx.lang.ArrayIndexOutOfBoundsException.h"

namespace jxx::awt::image {

PackedColorModel::PackedColorModel(
    ::jxx::lang::jint bits,
    ::jxx::lang::jint redMask,
    ::jxx::lang::jint greenMask,
    ::jxx::lang::jint blueMask,
    ::jxx::lang::jint alphaMask)
    : Super(bits),
      redMask_(redMask),
      greenMask_(greenMask),
      blueMask_(blueMask),
      alphaMask_(alphaMask) {
}

::jxx::lang::jint PackedColorModel::getMask(::jxx::lang::jint index) const {
    switch (index) {
    case 0: return redMask_;
    case 1: return greenMask_;
    case 2: return blueMask_;
    case 3: return alphaMask_;
    default: throw ::jxx::lang::ArrayIndexOutOfBoundsException(index);
    }
}

::jxx::lang::jint PackedColorModel::getRedMask() const { return redMask_; }
::jxx::lang::jint PackedColorModel::getGreenMask() const { return greenMask_; }
::jxx::lang::jint PackedColorModel::getBlueMask() const { return blueMask_; }
::jxx::lang::jint PackedColorModel::getAlphaMask() const { return alphaMask_; }

::jxx::lang::jint PackedColorModel::getTransparency() const {
    return alphaMask_ == 0
        ? ::jxx::awt::Transparency::OPAQUE
        : ::jxx::awt::Transparency::TRANSLUCENT;
}

::jxx::lang::jint PackedColorModel::extractComponent(
    ::jxx::lang::jint pixel,
    ::jxx::lang::jint mask) {
    if (mask == 0) return 0;
    auto remaining = static_cast<unsigned>(mask);
    auto value = static_cast<unsigned>(pixel) & remaining;
    while ((remaining & 1U) == 0U) {
        remaining >>= 1U;
        value >>= 1U;
    }
    return static_cast<::jxx::lang::jint>(
        remaining == 255U
            ? value
            : (value * 255U + (remaining / 2U)) / remaining);
}

} // namespace jxx::awt::image
