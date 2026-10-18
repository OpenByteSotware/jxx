#pragma once
#include "lang/jxx.lang.Object.h"
namespace jxx::awt::image { class ImageConsumer; class ImageProducer { public: virtual ~ImageProducer()=default; virtual void addConsumer(const ::jxx::Ptr<ImageConsumer>& consumer)=0; virtual ::jxx::lang::jbool isConsumer(const ::jxx::Ptr<ImageConsumer>& consumer)=0; virtual void removeConsumer(const ::jxx::Ptr<ImageConsumer>& consumer)=0; virtual void startProduction(const ::jxx::Ptr<ImageConsumer>& consumer)=0; virtual void requestTopDownLeftRightResend(const ::jxx::Ptr<ImageConsumer>& consumer)=0; }; }
