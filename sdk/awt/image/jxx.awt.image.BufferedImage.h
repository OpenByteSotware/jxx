#pragma once
#include "awt/jxx.awt.Image.h"
#include "lang/jxx.lang.buildin_array.h"
#include <cstdint>
#include <vector>
namespace jxx::awt { class Graphics2D; }
namespace jxx::awt::image {
class BufferedImage final : public ::jxx::lang::ClassBase<BufferedImage,::jxx::awt::Image> {
public:
 using JxxSuper=::jxx::awt::Image; using Super=::jxx::lang::ClassBase<BufferedImage,JxxSuper>;
 static constexpr ::jxx::lang::jint TYPE_CUSTOM=0,TYPE_INT_RGB=1,TYPE_INT_ARGB=2;
 BufferedImage(::jxx::lang::jint width,::jxx::lang::jint height,::jxx::lang::jint type);
 ::jxx::lang::jint getWidth(const ::jxx::Ptr<ImageObserver>&) const override{return width_;}
 ::jxx::lang::jint getHeight(const ::jxx::Ptr<ImageObserver>&) const override{return height_;}
 ::jxx::Ptr<ImageProducer> getSource() const override;
 ::jxx::Ptr<::jxx::awt::Graphics> getGraphics() override;
 ::jxx::Ptr<::jxx::awt::Graphics2D> createGraphics();
 ::jxx::Ptr<::jxx::lang::Object> getProperty(const ::jxx::Ptr<::jxx::lang::Object>&,const ::jxx::Ptr<ImageObserver>&) override{return nullptr;}
 ::jxx::lang::jint getRGB(::jxx::lang::jint x,::jxx::lang::jint y) const;
 void setRGB(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint rgb);
 ::jxx::Ptr<BufferedImage> getSubimage(::jxx::lang::jint x,::jxx::lang::jint y,::jxx::lang::jint width,::jxx::lang::jint height) const;
 const std::vector<std::uint32_t>& pixels()const{return pixels_;} std::vector<std::uint32_t>& pixels(){return pixels_;}
private: ::jxx::lang::jint width_,height_,type_; std::vector<std::uint32_t> pixels_;
}; }
