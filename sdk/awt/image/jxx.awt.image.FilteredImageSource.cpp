#include "awt/image/jxx.awt.image.FilteredImageSource.h"

#include <algorithm>

#include "awt/image/jxx.awt.image.ImageFilter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace jxx::awt::image {

FilteredImageSource::FilteredImageSource(
    const ::jxx::Ptr<ImageProducer>& source,
    const ::jxx::Ptr<ImageFilter>& filter)
    : source_(source), filter_(filter)
{
    if (source_ == nullptr) throw ::jxx::lang::NullPointerException("source");
    if (filter_ == nullptr) throw ::jxx::lang::NullPointerException("filter");
}

::jxx::Ptr<ImageFilter> FilteredImageSource::filterFor_(
    const ::jxx::Ptr<ImageConsumer>& consumer,
    ::jxx::lang::jbool create)
{
    if (consumer == nullptr) return nullptr;
    const auto found = std::find_if(consumers_.begin(), consumers_.end(),
        [&consumer](const ConsumerFilter& entry) {
            return entry.consumer == consumer;
        });
    if (found != consumers_.end()) return found->filter;
    if (!create) return nullptr;
    auto instance = filter_->getFilterInstance(consumer);
    if (instance != nullptr) consumers_.push_back({consumer, instance});
    return instance;
}

void FilteredImageSource::addConsumer(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    auto instance = filterFor_(consumer, true);
    if (instance != nullptr)
        source_->addConsumer(::jxx::CAST<ImageConsumer>(instance));
}

::jxx::lang::jbool FilteredImageSource::isConsumer(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    return filterFor_(consumer, false) != nullptr;
}

void FilteredImageSource::removeConsumer(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    auto instance = filterFor_(consumer, false);
    if (instance != nullptr)
        source_->removeConsumer(::jxx::CAST<ImageConsumer>(instance));
    consumers_.erase(std::remove_if(consumers_.begin(), consumers_.end(),
        [&consumer](const ConsumerFilter& entry) {
            return entry.consumer == consumer;
        }), consumers_.end());
}

void FilteredImageSource::startProduction(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    auto instance = filterFor_(consumer, true);
    if (instance != nullptr)
        source_->startProduction(::jxx::CAST<ImageConsumer>(instance));
}

void FilteredImageSource::requestTopDownLeftRightResend(
    const ::jxx::Ptr<ImageConsumer>& consumer)
{
    auto instance = filterFor_(consumer, false);
    if (instance != nullptr) instance->resendTopDownLeftRight(source_);
}

} // namespace jxx::awt::image
