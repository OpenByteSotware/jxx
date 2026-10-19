#include <turbojpeg.h>
#include <memory>
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
}
