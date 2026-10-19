#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Graphics2D.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"

TEST(BufferedImageGraphicsHeadless, FillRectUsesColorAndTranslation)
{
    ::jxx::initialize();
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->translate(2, 1);
    graphics->setColor(::jxx::awt::Color::red);
    graphics->fillRect(1, 2, 3, 2);

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(3, 3)), 0xffff0000U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(5, 4)), 0xffff0000U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 2)), 0xff000000U);
}

TEST(BufferedImageGraphicsHeadless, ClipRestrictsRasterWrites)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        6, 6, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->setColor(::jxx::awt::Color::green);
    graphics->setClip(2, 2, 2, 2);
    graphics->fillRect(0, 0, 6, 6);

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 2)), 0xff00ff00U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(3, 3)), 0xff00ff00U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(1, 2)), 0xff000000U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(4, 3)), 0xff000000U);
}

TEST(BufferedImageGraphicsHeadless, DrawLineIncludesBothEndpoints)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        5, 5, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->setColor(::jxx::awt::Color::blue);
    graphics->drawLine(0, 0, 4, 4);

    for (::jxx::lang::jint index = 0; index < 5; ++index)
        EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(index, index)),
            0xff0000ffU);
}

TEST(BufferedImageGraphicsHeadless, CreateCopiesIndependentGraphicsState)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        4, 4, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto parent = image->createGraphics();
    parent->setColor(::jxx::awt::Color::red);
    auto child = parent->create();
    child->translate(1, 1);
    child->setColor(::jxx::awt::Color::cyan);
    child->fillRect(0, 0, 1, 1);
    parent->fillRect(0, 0, 1, 1);

    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(0, 0)), 0xffff0000U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(1, 1)), 0xff00ffffU);
}
