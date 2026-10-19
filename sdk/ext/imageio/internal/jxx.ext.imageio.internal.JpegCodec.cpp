#include <turbojpeg.h>
#include <memory>
#include <vector>
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::ext::imageio::internal
{
	namespace
	{
		struct D
		{
			void operator()(void* p)const noexcept
			{
				if (p)tj3Destroy(p);
			}
		};
	} ::jxx::Ptr<::jxx::awt::image::BufferedImage> JpegCodec::decode(const ::jxx::lang::ByteArray& b)
	{
		if (!b || b->size() == 0)throw ::jxx::lang::IllegalArgumentException("JPEG data"); std::unique_ptr<void, D> h(tj3Init(TJINIT_DECOMPRESS)); if (!h || tj3DecompressHeader(h.get(), reinterpret_cast<const unsigned char*>(b->data()), b->size()) < 0)throw ::jxx::lang::IllegalArgumentException("Invalid JPEG"); int w = tj3Get(h.get(), TJPARAM_JPEGWIDTH), ht = tj3Get(h.get(), TJPARAM_JPEGHEIGHT); if (w <= 0 || ht <= 0)throw ::jxx::lang::IllegalArgumentException("JPEG dimensions"); std::vector<unsigned char> rgba(static_cast<size_t>(w) * ht * 4); if (tj3Decompress8(h.get(), reinterpret_cast<const unsigned char*>(b->data()), b->size(), rgba.data(), w * 4, TJPF_RGBA) < 0)throw ::jxx::lang::IllegalArgumentException("JPEG decode"); auto out = ::jxx::NEW<::jxx::awt::image::BufferedImage>(w, ht, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB); for (int y = 0; y < ht; ++y)for (int x = 0; x < w; ++x) {
			auto i = (static_cast<size_t>(y) * w + x) * 4; out->setRGB(x, y, static_cast<int>(0xff000000u | (rgba[i] << 16) | (rgba[i + 1] << 8) | rgba[i + 2]));
		}return out;
	}
    ::jxx::lang::ByteArray JpegCodec::encode(const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image, ::jxx::lang::jfloat quality)
    {
        if (image == nullptr) throw ::jxx::lang::IllegalArgumentException("image");
        std::unique_ptr<void, D> handle(tj3Init(TJINIT_COMPRESS));
        if (!handle) throw ::jxx::lang::IllegalArgumentException("JPEG encoder");
        const int width = image->getWidth(nullptr);
        const int height = image->getHeight(nullptr);
        std::vector<unsigned char> rgb(static_cast<std::size_t>(width) * height * 3U);
        for (int y = 0; y < height; ++y) for (int x = 0; x < width; ++x) {
            const auto value = static_cast<std::uint32_t>(image->getRGB(x, y));
            const auto offset = (static_cast<std::size_t>(y) * width + x) * 3U;
            rgb[offset] = static_cast<unsigned char>((value >> 16) & 0xffU);
            rgb[offset + 1] = static_cast<unsigned char>((value >> 8) & 0xffU);
            rgb[offset + 2] = static_cast<unsigned char>(value & 0xffU);
        }
        const int jpegQuality = static_cast<int>(quality * 100.0F + 0.5F);
        if (tj3Set(handle.get(), TJPARAM_QUALITY, jpegQuality < 1 ? 1 : (jpegQuality > 100 ? 100 : jpegQuality)) < 0 ||
            tj3Set(handle.get(), TJPARAM_SUBSAMP, TJSAMP_420) < 0)
            throw ::jxx::lang::IllegalArgumentException("JPEG parameters");
        unsigned char* encoded = nullptr; std::size_t encodedSize = 0;
        if (tj3Compress8(handle.get(), rgb.data(), width, width * 3, height, TJPF_RGB, &encoded, &encodedSize) < 0)
            throw ::jxx::lang::IllegalArgumentException("JPEG encode");
        struct BufferGuard { unsigned char* value; ~BufferGuard(){ if(value) tj3Free(value); } } guard{encoded};
        auto result = ::jxx::NEW<::jxx::lang::ByteArrayType>(static_cast<std::uint32_t>(encodedSize));
        for (std::size_t index = 0; index < encodedSize; ++index) (*result)[static_cast<::jxx::lang::jint>(index)] = static_cast<::jxx::lang::jbyte>(encoded[index]);
        return result;
    }
}

