#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Rectangle.h"
#include "awt/jxx.awt.Toolkit.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.FilteredImageSource.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"

namespace
{
class InvertFilter final
    : public ::jxx::lang::ClassBase<InvertFilter,
          ::jxx::awt::image::RGBImageFilter>
{
public:
    using JxxSuper = ::jxx::awt::image::RGBImageFilter;
    using Super = ::jxx::lang::ClassBase<InvertFilter, JxxSuper>;
    InvertFilter() : Super() {}

    ::jxx::lang::jint filterRGB(
        ::jxx::lang::jint,
        ::jxx::lang::jint,
        ::jxx::lang::jint rgb) override
    {
        const auto value = static_cast<std::uint32_t>(rgb);
        return static_cast<::jxx::lang::jint>(
            0xff000000U |
            ((0xffU - ((value >> 16) & 0xffU)) << 16) |
            ((0xffU - ((value >> 8) & 0xffU)) << 8) |
            (0xffU - (value & 0xffU)));
    }
};
}

TEST(HeadlessImagePipeline, FilteredImageSourceCreatesInvertedImage)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    source->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xffff0000U));
    source->setRGB(1, 0, static_cast<::jxx::lang::jint>(0xff00ff00U));

    auto filtered = std::make_shared<::jxx::awt::image::FilteredImageSource>(
        source->getSource(), ::jxx::NEW<InvertFilter>());
    auto result = ::jxx::CAST<::jxx::awt::image::BufferedImage>(
        ::jxx::awt::Toolkit::getDefaultToolkit()->createImage(filtered));

    ASSERT_NE(result, nullptr);
    EXPECT_EQ(static_cast<std::uint32_t>(result->getRGB(0, 0)), 0xff00ffffU);
    EXPECT_EQ(static_cast<std::uint32_t>(result->getRGB(1, 0)), 0xffff00ffU);
}

TEST(HeadlessImagePipeline, RectangleIntersectionMatchesExpectedBounds)
{
    auto left = ::jxx::NEW<::jxx::awt::Rectangle>(0, 0, 100, 80);
    auto right = ::jxx::NEW<::jxx::awt::Rectangle>(60, 40, 80, 80);
    auto common = left->intersection(right);
    ASSERT_NE(common, nullptr);
    EXPECT_EQ(common->x, 60);
    EXPECT_EQ(common->y, 40);
    EXPECT_EQ(common->width, 40);
    EXPECT_EQ(common->height, 40);
}

TEST(HeadlessImagePipeline, BufferedImageSourceReturnsOriginalImage)
{
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        3, 4, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto image = ::jxx::awt::Toolkit::getDefaultToolkit()->createImage(
        source->getSource());
    EXPECT_EQ(image, source);
}
