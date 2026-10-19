#pragma once
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::image {class BufferedImage;} namespace jxx::ext::imageio { class IIOImage final:public ::jxx::lang::ClassBase<IIOImage,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<IIOImage,JxxSuper>;IIOImage(const ::jxx::Ptr<::jxx::awt::image::BufferedImage>& image,const ::jxx::Ptr<::jxx::lang::Object>& thumbnails,const ::jxx::Ptr<::jxx::lang::Object>& metadata);::jxx::Ptr<::jxx::awt::image::BufferedImage> getRenderedImage()const{return image_;}private: ::jxx::Ptr<::jxx::awt::image::BufferedImage> image_;::jxx::Ptr<::jxx::lang::Object> thumbnails_,metadata_;};}
