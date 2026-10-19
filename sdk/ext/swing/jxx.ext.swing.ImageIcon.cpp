#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
namespace jxx::ext::swing{ImageIcon::ImageIcon(const ::jxx::lang::ByteArray&d):Super(),image_(::jxx::CAST<::jxx::awt::Image>(::jxx::ext::imageio::internal::JpegCodec::decode(d))){} ImageIcon::ImageIcon(const ::jxx::Ptr<::jxx::awt::Image>&i):Super(),image_(i){} int ImageIcon::getIconWidth()const{return image_?image_->getWidth(nullptr):-1;}int ImageIcon::getIconHeight()const{return image_?image_->getHeight(nullptr):-1;}}
