#pragma once
#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
namespace jxx::ext::imageio::internal{class JpegImageWriter final : public ::jxx::lang::ClassBase<JpegImageWriter, ::jxx::ext::imageio::ImageWriter>{public:using JxxSuper=::jxx::ext::imageio::ImageWriter;using Super=::jxx::lang::ClassBase<JpegImageWriter, JxxSuper>;JpegImageWriter();::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam> getDefaultWriteParam()override;void write(const ::jxx::Ptr<::jxx::lang::Object>&,const ::jxx::Ptr<::jxx::ext::imageio::IIOImage>&,const ::jxx::Ptr<::jxx::ext::imageio::ImageWriteParam>&)override;};}
