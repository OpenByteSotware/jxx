#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegImageWriter.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
#include "lang/jxx.lang.IllegalStateException.h"
namespace jxx::ext::imageio::internal{JpegImageWriter::JpegImageWriter():Super(){}::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam> JpegImageWriter::getDefaultWriteParam(){return ::jxx::NEW<::jxx::ext::imageio::ImageWriteParam>();}void JpegImageWriter::write(const ::jxx::Ptr<::jxx::lang::Object>&,const ::jxx::Ptr<::jxx::ext::imageio::IIOImage>&image,const ::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam>&param){auto output=::jxx::CAST<::jxx::ext::imageio::stream::ImageOutputStream>(output_);if(!output||!image)throw ::jxx::lang::IllegalStateException("output/image");float quality=param&&param->getCompressionMode()==::jxx::ext::imageio::ImageWriteParam::MODE_EXPLICIT?param->getCompressionQuality():0.75F;output->write(JpegCodec::encode(image->getRenderedImage(),quality));output->flush();}}
