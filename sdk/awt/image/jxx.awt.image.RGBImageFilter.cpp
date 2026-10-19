#include "awt/image/jxx.awt.image.RGBImageFilter.h"

#include "awt/image/jxx.awt.image.ColorModel.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt::image {

RGBImageFilter::RGBImageFilter() : Super() {}

::jxx::Ptr<ImageFilter> RGBImageFilter::getFilterInstance(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    if (consumer == nullptr) throw ::jxx::lang::NullPointerException("consumer");
    return ::jxx::NEW<RGBImageFilterInstance>(
        ::jxx::CAST<RGBImageFilter>(thisPtr()), consumer);
}

RGBImageFilterInstance::RGBImageFilterInstance(
    const ::jxx::Ptr<RGBImageFilter>& filter,
    const ::jxx::Ptr<ImageConsumer>& consumer)
    : ImageFilter(), filter_(filter)
{
    if (filter_ == nullptr) throw ::jxx::lang::NullPointerException("filter");
    if (consumer == nullptr) throw ::jxx::lang::NullPointerException("consumer");
    consumer_ = consumer;
}

void RGBImageFilterInstance::setPixels(::jxx::lang::jint x,
    ::jxx::lang::jint y, ::jxx::lang::jint width,
    ::jxx::lang::jint height, const ::jxx::Ptr<ColorModel>& model,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1>>& pixels,
    ::jxx::lang::jint offset, ::jxx::lang::jint scansize)
{
    if (consumer_ == nullptr || pixels == nullptr) return;
    auto output = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>(
        static_cast<std::uint32_t>(width * height));
    for (::jxx::lang::jint row = 0; row < height; ++row)
        for (::jxx::lang::jint column = 0; column < width; ++column) {
            const auto source = static_cast<unsigned char>(
                (*pixels)[offset + row * scansize + column]);
            const auto rgb = model == nullptr
                ? static_cast<::jxx::lang::jint>(source)
                : model->getRGB(source);
            (*output)[row * width + column] =
                filter_->filterRGB(x + column, y + row, rgb);
        }
    consumer_->setPixels(x, y, width, height, ColorModel::getRGBdefault(),
        output, 0, width);
}

void RGBImageFilterInstance::setPixels(::jxx::lang::jint x,
    ::jxx::lang::jint y, ::jxx::lang::jint width,
    ::jxx::lang::jint height, const ::jxx::Ptr<ColorModel>& model,
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>& pixels,
    ::jxx::lang::jint offset, ::jxx::lang::jint scansize)
{
    if (consumer_ == nullptr || pixels == nullptr) return;
    auto output = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>(
        static_cast<std::uint32_t>(width * height));
    for (::jxx::lang::jint row = 0; row < height; ++row)
        for (::jxx::lang::jint column = 0; column < width; ++column) {
            const auto source = (*pixels)[offset + row * scansize + column];
            const auto rgb = model == nullptr ? source : model->getRGB(source);
            (*output)[row * width + column] =
                filter_->filterRGB(x + column, y + row, rgb);
        }
    consumer_->setPixels(x, y, width, height, ColorModel::getRGBdefault(),
        output, 0, width);
}

} // namespace jxx::awt::image
