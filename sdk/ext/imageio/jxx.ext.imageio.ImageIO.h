#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::io{class OutputStream;} namespace jxx::lang{class String;} namespace jxx::util{template<typename E>class Iterator;} namespace jxx::ext::imageio{class ImageWriter;namespace stream{class ImageOutputStream;}class ImageIO final{public:static ::jxx::Ptr<::jxx::util::Iterator<ImageWriter>> getImageWritersByFormatName(const ::jxx::Ptr<::jxx::lang::String>& formatName);static ::jxx::Ptr<stream::ImageOutputStream> createImageOutputStream(const ::jxx::Ptr<::jxx::io::OutputStream>& output);};}
