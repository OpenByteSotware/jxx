#include <gtest/gtest.h>

#include <cstdint>

#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"

namespace {

TEST(BufferedImageSubimageParity, ChildAndParentSharePixelStorage)
{
    auto parent = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        4, 3, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    parent->setRGB(2, 1, static_cast<::jxx::lang::jint>(0xff123456U));
    auto child = parent->getSubimage(1, 1, 2, 2);
    ASSERT_NE(child, nullptr);
    EXPECT_EQ(child->getWidth(nullptr), 2);
    EXPECT_EQ(child->getHeight(nullptr), 2);
    EXPECT_EQ(static_cast<std::uint32_t>(child->getRGB(1, 0)), 0xff123456U);
    child->setRGB(0, 1, static_cast<::jxx::lang::jint>(0xffabcdefU));
    EXPECT_EQ(static_cast<std::uint32_t>(parent->getRGB(1, 2)), 0xffabcdefU);
    parent->setRGB(2, 2, static_cast<::jxx::lang::jint>(0xff010203U));
    EXPECT_EQ(static_cast<std::uint32_t>(child->getRGB(1, 1)), 0xff010203U);
}

TEST(BufferedImageSubimageParity, NestedSubimagesAccumulateOrigin)
{
    auto parent = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        5, 5, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    auto first = parent->getSubimage(1, 1, 3, 3);
    auto second = first->getSubimage(1, 1, 1, 1);
    second->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff778899U));
    EXPECT_EQ(static_cast<std::uint32_t>(parent->getRGB(2, 2)), 0xff778899U);
}

TEST(BufferedImageSubimageParity, RejectsInvalidRegions)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        3, 3, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    EXPECT_THROW(image->getSubimage(-1, 0, 1, 1),
        ::jxx::lang::IndexOutOfBoundsException);
    EXPECT_THROW(image->getSubimage(0, 0, 0, 1),
        ::jxx::lang::IndexOutOfBoundsException);
    EXPECT_THROW(image->getSubimage(2, 2, 2, 2),
        ::jxx::lang::IndexOutOfBoundsException);
}

TEST(BufferedImageSubimageParity, IntRgbForcesOpaqueAlpha)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        1, 1, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0x00123456U));
    EXPECT_EQ(static_cast<std::uint32_t>(image->getRGB(0, 0)), 0xff123456U);
}

} // namespace
