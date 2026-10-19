#include <gtest/gtest.h>

#include <cstdint>

#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.ColorModel.h"
#include "awt/image/jxx.awt.image.FilteredImageSource.h"
#include "awt/image/jxx.awt.image.ImageConsumer.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"

namespace {

class InvertFilter final : public ::jxx::awt::image::RGBImageFilter {
public:
    ::jxx::lang::jint filterRGB(::jxx::lang::jint,
        ::jxx::lang::jint, ::jxx::lang::jint rgb) override
    {
        const auto value = static_cast<std::uint32_t>(rgb);
        return static_cast<::jxx::lang::jint>(
            (value & 0xff000000U) | ((~value) & 0x00ffffffU));
    }
};

class CapturingConsumer final : public ::jxx::awt::image::ImageConsumer {
public:
    void setDimensions(::jxx::lang::jint width,
        ::jxx::lang::jint height) override { width_ = width; height_ = height; }
    void setProperties(const ::jxx::Ptr<::jxx::lang::Object>&) override {}
    void setColorModel(
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&) override {}
    void setHints(::jxx::lang::jint) override {}
    void setPixels(::jxx::lang::jint, ::jxx::lang::jint,
        ::jxx::lang::jint, ::jxx::lang::jint,
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1>>&,
        ::jxx::lang::jint, ::jxx::lang::jint) override {}
    void setPixels(::jxx::lang::jint, ::jxx::lang::jint,
        ::jxx::lang::jint width, ::jxx::lang::jint height,
        const ::jxx::Ptr<::jxx::awt::image::ColorModel>&,
        const ::jxx::Ptr<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>& pixels,
        ::jxx::lang::jint offset, ::jxx::lang::jint scansize) override
    {
        pixels_.clear();
        for (::jxx::lang::jint row = 0; row < height; ++row)
            for (::jxx::lang::jint column = 0; column < width; ++column)
                pixels_.push_back((*pixels)[offset + row * scansize + column]);
    }
    void imageComplete(::jxx::lang::jint status) override { status_ = status; }

    ::jxx::lang::jint width_ = 0;
    ::jxx::lang::jint height_ = 0;
    ::jxx::lang::jint status_ = 0;
    std::vector<::jxx::lang::jint> pixels_;
};

TEST(FilteredImageSourceParity, AppliesRgbFilterAndTracksConsumer)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff112233U));
    image->setRGB(1, 0, static_cast<::jxx::lang::jint>(0xffabcdefU));
    auto filtered = std::make_shared<::jxx::awt::image::FilteredImageSource>(
        image->getSource(), ::jxx::NEW<InvertFilter>());
    auto consumer = std::make_shared<CapturingConsumer>();

    EXPECT_FALSE(filtered->isConsumer(consumer));
    filtered->startProduction(consumer);
    EXPECT_TRUE(filtered->isConsumer(consumer));
    EXPECT_EQ(consumer->width_, 2);
    EXPECT_EQ(consumer->height_, 1);
    ASSERT_EQ(consumer->pixels_.size(), 2U);
    EXPECT_EQ(static_cast<std::uint32_t>(consumer->pixels_[0]), 0xffeeddccU);
    EXPECT_EQ(static_cast<std::uint32_t>(consumer->pixels_[1]), 0xff543210U);
    EXPECT_EQ(consumer->status_,
        ::jxx::awt::image::ImageConsumer::STATICIMAGEDONE);

    filtered->removeConsumer(consumer);
    EXPECT_FALSE(filtered->isConsumer(consumer));
}

} // namespace
