#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/geom/jxx.awt.geom.AffineTransform.h"
#include "awt/jxx.awt.Color.h"
#include "awt/jxx.awt.Graphics2D.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"

TEST(BufferedImageGraphicsTransform, ScaleAffectsRasterCoordinates)
{
    ::jxx::initialize();
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->setColor(::jxx::awt::Color::red);
    graphics->scale(2.0, 3.0);
    graphics->fillRect(1, 1, 1, 1);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(2, 3)), 0xffff0000U);
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(1, 1)), 0xff000000U);
}

TEST(BufferedImageGraphicsTransform, SetTransformCopiesInput)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    auto supplied = ::jxx::awt::geom::AffineTransform::getTranslateInstance(2.0, 3.0);
    graphics->setTransform(supplied);
    supplied->translate(4.0, 5.0);

    auto retained = graphics->getTransform();
    auto matrix = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble, 1>>(6);
    retained->getMatrix(matrix);
    EXPECT_DOUBLE_EQ((*matrix)[4], 2.0);
    EXPECT_DOUBLE_EQ((*matrix)[5], 3.0);
}

TEST(BufferedImageGraphicsTransform, GetTransformReturnsDefensiveCopy)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto graphics = image->createGraphics();
    graphics->scale(2.0, 3.0);
    auto copy = graphics->getTransform();
    copy->scale(5.0, 7.0);

    auto retained = graphics->getTransform();
    auto matrix = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble, 1>>(6);
    retained->getMatrix(matrix);
    EXPECT_DOUBLE_EQ((*matrix)[0], 2.0);
    EXPECT_DOUBLE_EQ((*matrix)[3], 3.0);
}

TEST(BufferedImageGraphicsTransform, CreateCopiesTransformState)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    auto parent = image->createGraphics();
    parent->scale(2.0, 2.0);
    auto child = ::jxx::CAST<::jxx::awt::Graphics2D>(parent->create());
    ASSERT_NE(child, nullptr);
    auto matrix = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jdouble, 1>>(6);
    child->getTransform()->getMatrix(matrix);
    EXPECT_DOUBLE_EQ((*matrix)[0], 2.0);
    EXPECT_DOUBLE_EQ((*matrix)[3], 2.0);
}
