#pragma once
#include "awt/image/jxx.awt.image.ImageFilter.h"
namespace jxx::awt::image{class RGBImageFilter:public ::jxx::lang::ClassBase<RGBImageFilter,ImageFilter>{public:using JxxSuper=ImageFilter;using Super=::jxx::lang::ClassBase<RGBImageFilter,JxxSuper>;RGBImageFilter();virtual ::jxx::lang::jint filterRGB(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint rgb)=0;};}
