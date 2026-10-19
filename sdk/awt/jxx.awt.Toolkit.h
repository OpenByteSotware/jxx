#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt{class Image;namespace image{class ImageProducer;}class Toolkit final:public ::jxx::lang::ClassBase<Toolkit,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<Toolkit,JxxSuper>;Toolkit();static ::jxx::Ptr<Toolkit> getDefaultToolkit();::jxx::Ptr<Image> createImage(const ::jxx::Ptr<::jxx::awt::image::ImageProducer>&producer);};}
