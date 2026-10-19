#pragma once
#include "awt/image/jxx.awt.image.ImageProducer.h"
#include <vector>
namespace jxx::awt::image {class BufferedImage;class ImageConsumer;}
namespace jxx::awt::image::internal {class BufferedImageSource final:public ImageProducer{public:explicit BufferedImageSource(const ::jxx::Ptr<BufferedImage>& image);void addConsumer(const ::jxx::Ptr<ImageConsumer>&)override;bool isConsumer(const ::jxx::Ptr<ImageConsumer>&)override;void removeConsumer(const ::jxx::Ptr<ImageConsumer>&)override;void startProduction(const ::jxx::Ptr<ImageConsumer>&)override;void requestTopDownLeftRightResend(const ::jxx::Ptr<ImageConsumer>&c)override{startProduction(c);}::jxx::Ptr<BufferedImage> image()const{return image_;}private: ::jxx::Ptr<BufferedImage> image_;std::vector<::jxx::Ptr<ImageConsumer>> consumers_;};}
