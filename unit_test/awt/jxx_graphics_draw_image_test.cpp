#include <gtest/gtest.h>

#include <cstdint>

#include "awt/jxx.awt.Graphics.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"

namespace {

TEST(GraphicsDrawImageParity, DrawsAndScalesBufferedImage)
{
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 2, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    source->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xffff0000U));
    source->setRGB(1, 0, static_cast<::jxx::lang::jint>(0xff00ff00U));
    source->setRGB(0, 1, static_cast<::jxx::lang::jint>(0xff0000ffU));
    source->setRGB(1, 1, static_cast<::jxx::lang::jint>(0xffffffffU));
    auto target = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        6, 6, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto graphics = target->getGraphics();
    ASSERT_NE(graphics, nullptr);
    EXPECT_TRUE(graphics->drawImage(source, 1, 1, 4, 4, nullptr));
    graphics->dispose();
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(1, 1)), 0xffff0000U);
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(4, 1)), 0xff00ff00U);
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(1, 4)), 0xff0000ffU);
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(4, 4)), 0xffffffffU);
}

TEST(GraphicsDrawImageParity, DrawsSelectedSourceRegion)
{
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        3, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    source->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xffff0000U));
    source->setRGB(1, 0, static_cast<::jxx::lang::jint>(0xff00ff00U));
    source->setRGB(2, 0, static_cast<::jxx::lang::jint>(0xff0000ffU));
    auto target = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto graphics = target->getGraphics();
    ASSERT_NE(graphics, nullptr);
    EXPECT_TRUE(graphics->drawImage(source, 0, 0, 2, 1,
        1, 0, 3, 1, nullptr));
    graphics->dispose();
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(0, 0)), 0xff00ff00U);
    EXPECT_EQ(static_cast<std::uint32_t>(target->getRGB(1, 0)), 0xff0000ffU);
}

} // namespace
