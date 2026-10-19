#include "ext/swing/jxx.ext.swing.ImageIcon.h"

#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

namespace jxx::ext::swing {

ImageIcon::ImageIcon()
    : Super(), image_(nullptr), description_(nullptr) {}

ImageIcon::ImageIcon(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data)
    : ImageIcon(data, nullptr) {}

ImageIcon::ImageIcon(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data,
    const ::jxx::Ptr<::jxx::lang::String>& description)
    : Super(), image_(nullptr), description_(description) {
    decode_(data);
}

ImageIcon::ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>& image)
    : ImageIcon(image, nullptr) {}

ImageIcon::ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>& image,
    const ::jxx::Ptr<::jxx::lang::String>& description)
    : Super(), image_(image), description_(description) {}

void ImageIcon::decode_(
    const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>& data) {
    image_.reset();
    if (data == nullptr || data->length == 0) return;
    try {
        image_ = ::jxx::CAST<::jxx::awt::Image>(
            ::jxx::ext::imageio::internal::JpegCodec::decode(data));
    } catch (...) {
        image_.reset();
    }
}

::jxx::Ptr<::jxx::awt::Image> ImageIcon::getImage() const { return image_; }

void ImageIcon::setImage(const ::jxx::Ptr<::jxx::awt::Image>& image) {
    image_ = image;
}

::jxx::Ptr<::jxx::lang::String> ImageIcon::getDescription() const {
    return description_;
}

void ImageIcon::setDescription(
    const ::jxx::Ptr<::jxx::lang::String>& description) {
    description_ = description;
}

::jxx::lang::jint ImageIcon::getIconWidth() const {
    return image_ == nullptr ? -1 : image_->getWidth(nullptr);
}

::jxx::lang::jint ImageIcon::getIconHeight() const {
    return image_ == nullptr ? -1 : image_->getHeight(nullptr);
}

void ImageIcon::writeObject(
    const ::jxx::Ptr<::jxx::io::ObjectOutputStream>& out) {
    if (out == nullptr) return;
    out->writeObject(description_);
    const auto buffered = ::jxx::CAST<::jxx::awt::image::BufferedImage>(image_);
    if (buffered == nullptr) {
        out->writeInt(-1);
        out->writeInt(-1);
        return;
    }
    const auto width = buffered->getWidth(nullptr);
    const auto height = buffered->getHeight(nullptr);
    out->writeInt(width);
    out->writeInt(height);
    for (::jxx::lang::jint y = 0; y < height; ++y)
        for (::jxx::lang::jint x = 0; x < width; ++x)
            out->writeInt(buffered->getRGB(x, y));
}

void ImageIcon::readObject(
    const ::jxx::Ptr<::jxx::io::ObjectInputStream>& in) {
    readObjectNoData();
    if (in == nullptr) return;
    description_ = ::jxx::CAST<::jxx::lang::String>(in->readObject());
    const auto width = in->readInt();
    const auto height = in->readInt();
    if (width < 0 || height < 0) return;
    auto buffered = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        width, height, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    for (::jxx::lang::jint y = 0; y < height; ++y)
        for (::jxx::lang::jint x = 0; x < width; ++x)
            buffered->setRGB(x, y, in->readInt());
    image_ = ::jxx::CAST<::jxx::awt::Image>(buffered);
}

void ImageIcon::readObjectNoData() {
    image_.reset();
    description_.reset();
}

} // namespace jxx::ext::swing
