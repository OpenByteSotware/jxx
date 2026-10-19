#include "ext/imageio/jxx.ext.imageio.ImageIO.h"

#include <cctype>
#include <memory>

#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegImageWriter.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.MemoryCacheImageOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"
#include "util/jxx.util.NoSuchElementException.h"

namespace jxx::ext::imageio {
namespace {

class SingleWriterIterator final : public ::jxx::util::Iterator<ImageWriter> {
public:
    explicit SingleWriterIterator(const ::jxx::Ptr<ImageWriter>& writer)
        : writer_(writer) {}

    ::jxx::lang::jbool hasNext() override { return writer_ != nullptr; }

    ::jxx::Ptr<ImageWriter> next() override
    {
        if (writer_ == nullptr) throw ::jxx::util::NoSuchElementException();
        auto result = writer_;
        writer_.reset();
        return result;
    }

private:
    ::jxx::Ptr<ImageWriter> writer_;
};

} // namespace

::jxx::Ptr<::jxx::util::Iterator<ImageWriter>>
ImageIO::getImageWritersByFormatName(
    const ::jxx::Ptr<::jxx::lang::String>& formatName)
{
    if (formatName == nullptr)
        throw ::jxx::lang::IllegalArgumentException("formatName");
    auto text = formatName->utf8();
    for (auto& character : text)
        character = static_cast<char>(
            std::tolower(static_cast<unsigned char>(character)));
    auto writer = text == "jpeg" || text == "jpg"
        ? ::jxx::CAST<ImageWriter>(
            ::jxx::NEW<::jxx::ext::imageio::internal::JpegImageWriter>())
        : nullptr;
    return std::make_shared<SingleWriterIterator>(writer);
}

::jxx::Ptr<stream::ImageOutputStream> ImageIO::createImageOutputStream(
    const ::jxx::Ptr<::jxx::io::OutputStream>& output)
{
    if (output == nullptr) throw ::jxx::lang::IllegalArgumentException("output");
    return ::jxx::NEW<stream::MemoryCacheImageOutputStream>(output);
}

} // namespace jxx::ext::imageio
