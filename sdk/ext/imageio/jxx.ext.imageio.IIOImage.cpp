#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
namespace jxx::ext::imageio{IIOImage::IIOImage(const ::jxx::Ptr<::jxx::awt::image::BufferedImage>&i,const ::jxx::Ptr<::jxx::lang::Object>&t,const ::jxx::Ptr<::jxx::lang::Object>&m):Super(),image_(i),thumbnails_(t),metadata_(m){if(!i)throw ::jxx::lang::IllegalArgumentException("image");}}
