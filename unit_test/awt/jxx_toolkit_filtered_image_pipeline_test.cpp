#include <gtest/gtest.h>

#include <cstdint>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Graphics2D.h"
#include "awt/jxx.awt.Toolkit.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.FilteredImageSource.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"
#include "lang/jxx.lang.NullPointerException.h"

namespace {

class InvertRgbFilter final : public ::jxx::awt::image::RGBImageFilter {
public:
    ::jxx::lang::jint filterRGB(::jxx::lang::jint,
        ::jxx::lang::jint, ::jxx::lang::jint rgb) override
    {
        const auto value = static_cast<std::uint32_t>(rgb);
        const auto red = 0xffU - ((value >> 16U) & 0xffU);
        const auto green = 0xffU - ((value >> 8U) & 0xffU);
        const auto blue = 0xffU - (value & 0xffU);
        return static_cast<::jxx::lang::jint>(
            0xff000000U | (red << 16U) | (green << 8U) | blue);
    }
};

TEST(ToolkitFilteredImageParity, MaterializesInvertedImageAndPreservesSource)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    source->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff112233U));
    source->setRGB(1, 0, static_cast<::jxx::lang::jint>(0xffabcdefU));
    auto producer = std::make_shared<::jxx::awt::image::FilteredImageSource>(
        source->getSource(), ::jxx::NEW<InvertRgbFilter>());

    auto image = ::jxx::awt::Toolkit::getDefaultToolkit()->createImage(producer);
    auto filtered = ::jxx::CAST<::jxx::awt::image::BufferedImage>(image);
    ASSERT_NE(filtered, nullptr);
    EXPECT_EQ(filtered->getWidth(nullptr), 2);
    EXPECT_EQ(filtered->getHeight(nullptr), 1);
    EXPECT_EQ(static_cast<std::uint32_t>(filtered->getRGB(0, 0)), 0xffeeddccU);
    EXPECT_EQ(static_cast<std::uint32_t>(filtered->getRGB(1, 0)), 0xff543210U);
    EXPECT_EQ(static_cast<std::uint32_t>(source->getRGB(0, 0)), 0xff112233U);
}

TEST(ToolkitFilteredImageParity, ResultCanBeDrawnIntoAnotherBufferedImage)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        1, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    source->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff000000U));
    auto producer = std::make_shared<::jxx::awt::image::FilteredImageSource>(
        source->getSource(), ::jxx::NEW<InvertRgbFilter>());
    auto filtered = ::jxx::awt::Toolkit::getDefaultToolkit()->createImage(producer);
    auto target = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 2, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto graphics = target->createGraphics();
    ASSERT_NE(graphics, nullptr);
    EXPECT_TRUE(graphics->drawImage(filtered, 1, 1, nullptr));
    graphics->dispose();
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(1, 1)), 0xffffffffU);
}

TEST(ToolkitFilteredImageParity, RejectsNullProducer)
{
    ::jxx::initialize();
    EXPECT_THROW(
        ::jxx::awt::Toolkit::getDefaultToolkit()->createImage(nullptr),
        ::jxx::lang::NullPointerException);
}

} // namespace
