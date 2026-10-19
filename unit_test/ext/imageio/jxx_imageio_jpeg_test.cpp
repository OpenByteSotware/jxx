#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageIO.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "lang/jxx.lang.String.h"
#include "util/jxx.util.Iterator.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"

TEST(ImageWriteParamParity, ExplicitQualityValidation)
{
    ::jxx::initialize();
    auto parameter = ::jxx::NEW<::jxx::ext::imageio::ImageWriteParam>();
    EXPECT_THROW(parameter->setCompressionQuality(0.85F), ::jxx::lang::IllegalStateException);
    parameter->setCompressionMode(::jxx::ext::imageio::ImageWriteParam::MODE_EXPLICIT);
    parameter->setCompressionQuality(0.85F);
    EXPECT_FLOAT_EQ(parameter->getCompressionQuality(), 0.85F);
    EXPECT_THROW(parameter->setCompressionQuality(1.01F), ::jxx::lang::IllegalArgumentException);
}

TEST(ImageIOJpegParity, WriterRoundTripsThroughImageIcon)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        8, 8, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    for (::jxx::lang::jint y = 0; y < 8; ++y)
        for (::jxx::lang::jint x = 0; x < 8; ++x)
            source->setRGB(x, y, x < 4 ? 0xffff0000 : 0xff0000ff);

    auto writers = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("jpeg"));
    ASSERT_TRUE(writers->hasNext());
    auto writer = writers->next();
    ASSERT_NE(writer, nullptr);
    EXPECT_FALSE(writers->hasNext());

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto imageOutput = ::jxx::ext::imageio::ImageIO::createImageOutputStream(bytes);
    writer->setOutput(::jxx::CAST<::jxx::lang::Object>(imageOutput));
    auto parameter = writer->getDefaultWriteParam();
    parameter->setCompressionMode(::jxx::ext::imageio::ImageWriteParam::MODE_EXPLICIT);
    parameter->setCompressionQuality(0.85F);
    writer->write(nullptr, ::jxx::NEW<::jxx::ext::imageio::IIOImage>(source, nullptr, nullptr), parameter);
    writer->dispose();
    imageOutput->close();

    auto encoded = bytes->toByteArray();
    ASSERT_NE(encoded, nullptr);
    EXPECT_GT(encoded->size(), 2U);
    EXPECT_EQ(static_cast<unsigned char>((*encoded)[0]), 0xffU);
    EXPECT_EQ(static_cast<unsigned char>((*encoded)[1]), 0xd8U);

    auto icon = ::jxx::NEW<::jxx::ext::swing::ImageIcon>(encoded);
    ASSERT_NE(icon->getImage(), nullptr);
    EXPECT_EQ(icon->getIconWidth(), 8);
    EXPECT_EQ(icon->getIconHeight(), 8);
}

TEST(ImageIOJpegParity, UnknownFormatHasNoWriter)
{
    auto writers = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("unknown"));
    ASSERT_NE(writers, nullptr);
    EXPECT_FALSE(writers->hasNext());
}
