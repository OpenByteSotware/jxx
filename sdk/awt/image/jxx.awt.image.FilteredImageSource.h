#pragma once

#include <vector>

#include "awt/image/jxx.awt.image.ImageProducer.h"

namespace jxx::awt::image {

class ImageFilter;

class FilteredImageSource final : public ImageProducer {
public:
    FilteredImageSource(const ::jxx::Ptr<ImageProducer>& source,
        const ::jxx::Ptr<ImageFilter>& filter);

    void addConsumer(const ::jxx::Ptr<ImageConsumer>& consumer) override;
    ::jxx::lang::jbool isConsumer(
        const ::jxx::Ptr<ImageConsumer>& consumer) override;
    void removeConsumer(const ::jxx::Ptr<ImageConsumer>& consumer) override;
    void startProduction(const ::jxx::Ptr<ImageConsumer>& consumer) override;
    void requestTopDownLeftRightResend(
        const ::jxx::Ptr<ImageConsumer>& consumer) override;

    ::jxx::Ptr<ImageProducer> source() const { return source_; }
    ::jxx::Ptr<ImageFilter> filter() const { return filter_; }

private:
    struct ConsumerFilter final {
        ::jxx::Ptr<ImageConsumer> consumer;
        ::jxx::Ptr<ImageFilter> filter;
    };

    ::jxx::Ptr<ImageFilter> filterFor_(
        const ::jxx::Ptr<ImageConsumer>& consumer,
        ::jxx::lang::jbool create);

    ::jxx::Ptr<ImageProducer> source_;
    ::jxx::Ptr<ImageFilter> filter_;
    std::vector<ConsumerFilter> consumers_;
};

} // namespace jxx::awt::image
