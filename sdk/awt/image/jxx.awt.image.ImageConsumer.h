#pragma once
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx_types.h"
namespace jxx::awt::image { class ColorModel; class ImageConsumer { public:
 static constexpr ::jxx::lang::jint RANDOMPIXELORDER=1, TOPDOWNLEFTRIGHT=2, COMPLETESCANLINES=4, SINGLEPASS=8, SINGLEFRAME=16;
 static constexpr ::jxx::lang::jint IMAGEERROR=1, SINGLEFRAMEDONE=2, STATICIMAGEDONE=3, IMAGEABORTED=4;
 virtual ~ImageConsumer()=default; virtual void setDimensions(::jxx::lang::jint width,::jxx::lang::jint height)=0; virtual void setProperties(const ::jxx::Ptr<::jxx::lang::Object>& properties)=0; virtual void setColorModel(const ::jxx::Ptr<ColorModel>& model)=0; virtual void setHints(::jxx::lang::jint hintflags)=0;
 virtual void setPixels(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height,const ::jxx::Ptr<ColorModel>& model,const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte,1>>& pixels,::jxx::lang::jint offset,::jxx::lang::jint scansize)=0;
 virtual void setPixels(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height,const ::jxx::Ptr<ColorModel>& model,const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint,1>>& pixels,::jxx::lang::jint offset,::jxx::lang::jint scansize)=0; virtual void imageComplete(::jxx::lang::jint status)=0; }; }
