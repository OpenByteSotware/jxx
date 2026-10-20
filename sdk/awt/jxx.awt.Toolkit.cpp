#include "awt/jxx.awt.Toolkit.h"

#include "awt/image/internal/jxx.awt.image.internal.BufferedImageSource.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.FilteredImageSource.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt {

Toolkit::Toolkit() : Super() {}

::jxx::Ptr<Toolkit> Toolkit::getDefaultToolkit()
{
    static auto value = ::jxx::NEW<Toolkit>();
    return value;
}

::jxx::Ptr<Image> Toolkit::createImage(
    const ::jxx::Ptr<::jxx::awt::image::ImageProducer>& producer)
{
    if (producer == nullptr)
        throw ::jxx::lang::NullPointerException("producer");

    auto bufferedSource = std::dynamic_pointer_cast<
        ::jxx::awt::image::internal::BufferedImageSource>(producer);
    if (bufferedSource != nullptr)
        return ::jxx::CAST<Image>(bufferedSource->image());

    auto filteredSource = std::dynamic_pointer_cast<
        ::jxx::awt::image::FilteredImageSource>(producer);
    if (filteredSource == nullptr) return nullptr;

    auto filter = ::jxx::CAST<::jxx::awt::image::RGBImageFilter>(
        filteredSource->filter());
    if (filter == nullptr) return nullptr;

    auto sourceImage = createImage(filteredSource->source());
    auto input = ::jxx::CAST<::jxx::awt::image::BufferedImage>(sourceImage);
    if (input == nullptr) return nullptr;

    const auto width = input->getWidth(nullptr);
    const auto height = input->getHeight(nullptr);
    auto output = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        width, height, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    for (::jxx::lang::jint y = 0; y < height; ++y) {
        for (::jxx::lang::jint x = 0; x < width; ++x) {
            output->setRGB(x, y,
                filter->filterRGB(x, y, input->getRGB(x, y)));
        }
    }
    return ::jxx::CAST<Image>(output);
}

} // namespace jxx::awt
