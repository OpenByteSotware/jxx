#include "awt/image/jxx.awt.image.ColorModel.h"
#include "awt/image/jxx.awt.image.DirectColorModel.h"
namespace jxx::awt::image { ColorModel::ColorModel(::jxx::lang::jint bits):pixelBits_(bits){} ::jxx::lang::jint ColorModel::getRGB(::jxx::lang::jint p) const{return (getAlpha(p)<<24)|(getRed(p)<<16)|(getGreen(p)<<8)|getBlue(p);} ::jxx::lang::jint ColorModel::getPixelSize() const{return pixelBits_;} ::jxx::Ptr<ColorModel> ColorModel::getRGBdefault(){static auto model=::jxx::NEW<DirectColorModel>(32,0x00ff0000,0x0000ff00,0x000000ff,static_cast<::jxx::lang::jint>(0xff000000u));return model;} }
