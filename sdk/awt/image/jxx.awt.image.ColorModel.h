#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "awt/jxx.awt.Transparency.h"
#include "lang/jxx_types.h"
namespace jxx::awt::image { class ColorModel : public ::jxx::lang::ClassBase<ColorModel,::jxx::lang::Object,::jxx::awt::Transparency> { public: using JxxSuper=::jxx::lang::Object; using Super=::jxx::lang::ClassBase<ColorModel,JxxSuper,::jxx::awt::Transparency>; explicit ColorModel(::jxx::lang::jint bits); virtual ::jxx::lang::jint getRed(::jxx::lang::jint pixel) const=0; virtual ::jxx::lang::jint getGreen(::jxx::lang::jint pixel) const=0; virtual ::jxx::lang::jint getBlue(::jxx::lang::jint pixel) const=0; virtual ::jxx::lang::jint getAlpha(::jxx::lang::jint pixel) const=0; virtual ::jxx::lang::jint getRGB(::jxx::lang::jint pixel) const; ::jxx::lang::jint getPixelSize() const; static ::jxx::Ptr<ColorModel> getRGBdefault(); protected: ::jxx::lang::jint pixelBits_; }; }
