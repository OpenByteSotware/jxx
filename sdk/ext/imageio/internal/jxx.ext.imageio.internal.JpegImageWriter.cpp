#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegImageWriter.h"

#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"

namespace jxx::ext::imageio::internal {

JpegImageWriter::JpegImageWriter() : Super() {}

::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam>
JpegImageWriter::getDefaultWriteParam()
{
    return ::jxx::NEW<::jxx::ext::imageio::ImageWriteParam>();
}

void JpegImageWriter::write(
    const ::jxx::Ptr<::jxx::lang::Object>&,
    const ::jxx::Ptr<::jxx::ext::imageio::IIOImage>& image,
    const ::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam>& parameter)
{
    auto output = ::jxx::CAST<
        ::jxx::ext::imageio::stream::ImageOutputStream>(output_);
    if (output == nullptr)
        throw ::jxx::lang::IllegalStateException("output not set");
    if (image == nullptr || image->getRenderedImage() == nullptr)
        throw ::jxx::lang::IllegalArgumentException("image");

    const auto quality = parameter != nullptr &&
        parameter->getCompressionMode() ==
            ::jxx::ext::imageio::ImageWriteParam::MODE_EXPLICIT
        ? parameter->getCompressionQuality()
        : 0.75F;
    output->write(JpegCodec::encode(image->getRenderedImage(), quality));
    output->flush();
}

} // namespace jxx::ext::imageio::internal
