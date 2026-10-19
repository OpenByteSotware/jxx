#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
namespace jxx::ext::imageio{ImageWriter::ImageWriter():Super(){}void ImageWriter::setOutput(const ::jxx::Ptr<::jxx::lang::Object>&o){output_=o;}void ImageWriter::dispose(){output_=nullptr;}}
