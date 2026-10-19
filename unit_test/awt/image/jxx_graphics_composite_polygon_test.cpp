#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/jxx.awt.AlphaComposite.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Graphics2D.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/jxx.awt.Composite.h"
#include "awt/jxx.awt.Graphics.h"

TEST(BufferedImageGraphicsComposite, SrcOverBlendsTransparentColor)
{
    ::jxx::initialize();
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        1, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff0000ffU));
    auto graphics = image->createGraphics();
    graphics->setComposite(::jxx::awt::AlphaComposite::SrcOver);
    graphics->setColor(::jxx::NEW<::jxx::awt::Color>(255, 0, 0, 128));
    graphics->fillRect(0, 0, 1, 1);
    const auto pixel = static_cast<std::uint32_t>(image->getRGB(0, 0));
    EXPECT_EQ((pixel >> 24U) & 0xffU, 0xffU);
    EXPECT_NEAR((pixel >> 16U) & 0xffU, 128U, 1U);
    EXPECT_NEAR(pixel & 0xffU, 127U, 1U);
}

TEST(BufferedImageGraphicsComposite, ClearRemovesDestination)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        1, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xffffffffU));
    auto graphics = image->createGraphics();
    graphics->setComposite(::jxx::awt::AlphaComposite::Clear);
    graphics->fillRect(0, 0, 1, 1);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(0, 0)), 0U);
}

TEST(BufferedImageGraphicsPolygon, FillPolygonFillsInterior)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->setColor(::jxx::awt::Color::green);
    auto xs = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>(3);
    auto ys = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jint, 1>>(3);
    (*xs)[0] = 1; (*ys)[0] = 1;
    (*xs)[1] = 6; (*ys)[1] = 1;
    (*xs)[2] = 3; (*ys)[2] = 6;
    graphics->fillPolygon(xs, ys, 3);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(3, 3)), 0xff00ff00U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(0, 0)), 0xff000000U);
}

TEST(BufferedImageGraphicsComposite, CreateCopiesCompositeState)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        1, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto parent = image->createGraphics();
    parent->setComposite(::jxx::awt::AlphaComposite::Clear);
    auto child = ::jxx::CAST<::jxx::awt::Graphics2D>(
    parent->create());

    ASSERT_NE(child, nullptr);
    EXPECT_EQ(child->getComposite(),::jxx::awt::AlphaComposite::Clear);
     
}
