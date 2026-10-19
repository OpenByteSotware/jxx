#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::ext::imageio { ImageWriteParam::ImageWriteParam():Super(){} void ImageWriteParam::setCompressionMode(::jxx::lang::jint mode){if(mode<MODE_DISABLED||mode>MODE_COPY_FROM_METADATA)throw ::jxx::lang::IllegalArgumentException("mode");mode_=mode;} void ImageWriteParam::setCompressionQuality(::jxx::lang::jfloat q){if(mode_!=MODE_EXPLICIT)throw ::jxx::lang::IllegalStateException("compression mode");if(q<0.0F||q>1.0F)throw ::jxx::lang::IllegalArgumentException("quality");quality_=q;} }
