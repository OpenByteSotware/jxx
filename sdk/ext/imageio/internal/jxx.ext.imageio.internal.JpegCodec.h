#pragma once
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::awt::image{class BufferedImage;} namespace jxx::ext::imageio::internal{class JpegCodec final{public: static ::jxx::Ptr<::jxx::awt::image::BufferedImage> decode(const ::jxx::lang::ByteArray& bytes);
    static ::jxx::lang::ByteArray encode(const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image, ::jxx::lang::jfloat quality);};}
