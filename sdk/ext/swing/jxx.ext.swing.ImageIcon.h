#pragma once
#include "awt/jxx.awt.Image.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.buildin_array.h"
namespace jxx::ext::swing{class ImageIcon final:public ::jxx::lang::ClassBase<ImageIcon,::jxx::lang::Object>{public:using JxxSuper=::jxx::lang::Object;using Super=::jxx::lang::ClassBase<ImageIcon,JxxSuper>;explicit ImageIcon(const ::jxx::lang::ByteArray& data);explicit ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>& image);::jxx::Ptr<::jxx::awt::Image> getImage()const{return image_;}int getIconWidth()const;int getIconHeight()const;private: ::jxx::Ptr<::jxx::awt::Image> image_;};}
