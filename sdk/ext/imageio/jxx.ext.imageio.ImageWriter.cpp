#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"

#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"

namespace jxx::ext::imageio {

ImageWriter::ImageWriter() : Super() {}

void ImageWriter::setOutput(const ::jxx::Ptr<::jxx::lang::Object>& output)
{
    if (output != nullptr &&
        ::jxx::CAST<::jxx::ext::imageio::stream::ImageOutputStream>(output) == nullptr) {
        throw ::jxx::lang::IllegalArgumentException("output");
    }
    output_ = output;
}

void ImageWriter::dispose()
{
    output_.reset();
}

} // namespace jxx::ext::imageio
