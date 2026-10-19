#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/internal/jxx.awt.internal.BufferedImageGraphics2D.h"
#include "awt/image/internal/jxx.awt.image.internal.BufferedImageSource.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include <memory>
namespace jxx::awt::image {
BufferedImage::BufferedImage(::jxx::lang::jint w,::jxx::lang::jint h,::jxx::lang::jint t):Super(),width_(w),height_(h),type_(t),pixels_(static_cast<std::size_t>(w)*static_cast<std::size_t>(h),t==TYPE_INT_RGB?0xff000000u:0u){if(w<=0||h<=0)throw ::jxx::lang::IllegalArgumentException("width/height");}
::jxx::Ptr<ImageProducer> BufferedImage::getSource()const{return std::make_shared<::jxx::awt::image::internal::BufferedImageSource>(::jxx::CAST<BufferedImage>(thisPtr()));}
::jxx::Ptr<::jxx::awt::Graphics> BufferedImage::getGraphics(){return createGraphics();}
::jxx::Ptr<::jxx::awt::Graphics2D> BufferedImage::createGraphics(){return ::jxx::NEW<::jxx::awt::internal::BufferedImageGraphics2D>(::jxx::CAST<BufferedImage>(thisPtr()));}
::jxx::lang::jint BufferedImage::getRGB(::jxx::lang::jint x,::jxx::lang::jint y)const{if(x<0||y<0||x>=width_||y>=height_)throw ::jxx::lang::IndexOutOfBoundsException();return static_cast<::jxx::lang::jint>(pixels_[static_cast<std::size_t>(y)*width_+x]);}
void BufferedImage::setRGB(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint rgb){if(x<0||y<0||x>=width_||y>=height_)throw ::jxx::lang::IndexOutOfBoundsException();pixels_[static_cast<std::size_t>(y)*width_+x]=static_cast<std::uint32_t>(rgb);}
::jxx::Ptr<BufferedImage> BufferedImage::getSubimage(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint w,::jxx::lang::jint h)const{auto out=::jxx::NEW<BufferedImage>(w,h,type_);for(int yy=0;yy<h;++yy)for(int xx=0;xx<w;++xx)out->setRGB(xx,yy,getRGB(x+xx,y+yy));return out;}
}
