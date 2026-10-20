#include <gtest/gtest.h>

#include "jxx.initialize.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/imageio/jxx.ext.imageio.IIOImage.h"
#include "ext/imageio/jxx.ext.imageio.ImageIO.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriteParam.h"
#include "ext/imageio/jxx.ext.imageio.ImageWriter.h"
#include "ext/imageio/stream/jxx.ext.imageio.stream.ImageOutputStream.h"
#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IllegalStateException.h"
#include "util/jxx.util.Iterator.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

TEST(ImageIOCacheManagerParity, EncodesSubimageAndImageIconDecodesIt)
{
    ::jxx::initialize();
    auto source = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        4, 3, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    source->setRGB(1, 1, static_cast<::jxx::lang::jint>(0xff336699U));
    auto subimage = source->getSubimage(1, 1, 2, 1);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto stream = ::jxx::ext::imageio::ImageIO::createImageOutputStream(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));
    auto writers = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("jpeg"));
    auto writer = writers->next();
    auto parameter = writer->getDefaultWriteParam();
    parameter->setCompressionMode(
        ::jxx::ext::imageio::ImageWriteParam::MODE_EXPLICIT);
    parameter->setCompressionQuality(0.85F);
    writer->setOutput(::jxx::CAST<::jxx::lang::Object>(stream));
    writer->write(nullptr,
        ::jxx::NEW<::jxx::ext::imageio::IIOImage>(subimage, nullptr, nullptr),
        parameter);
    writer->dispose();
    stream->close();

    auto encoded = bytes->toByteArray();
    ASSERT_NE(encoded, nullptr);
    EXPECT_GT(encoded->length, 0);
    auto icon = ::jxx::NEW<::jxx::ext::swing::ImageIcon>(encoded);
    EXPECT_EQ(icon->getIconWidth(), 2);
    EXPECT_EQ(icon->getIconHeight(), 1);
}

TEST(ImageWriterParity, ValidatesOutputAndWriteState)
{
    ::jxx::initialize();
    auto writers = ::jxx::ext::imageio::ImageIO::getImageWritersByFormatName(
        ::jxx::NEW<::jxx::lang::String>("jpeg"));
    auto writer = writers->next();
    EXPECT_THROW(writer->setOutput(::jxx::NEW<::jxx::lang::Object>()),
        ::jxx::lang::IllegalArgumentException);
    EXPECT_THROW(writer->write(nullptr, nullptr, nullptr),
        ::jxx::lang::IllegalStateException);

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    auto stream = ::jxx::ext::imageio::ImageIO::createImageOutputStream(
        ::jxx::CAST<::jxx::io::OutputStream>(bytes));
    writer->setOutput(::jxx::CAST<::jxx::lang::Object>(stream));
    EXPECT_THROW(writer->write(nullptr, nullptr, nullptr),
        ::jxx::lang::IllegalArgumentException);
}
