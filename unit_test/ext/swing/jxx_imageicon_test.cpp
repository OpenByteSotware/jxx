#include <gtest/gtest.h>

#include <cstdint>

#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "ext/imageio/internal/jxx.ext.imageio.internal.JpegCodec.h"
#include "ext/swing/jxx.ext.swing.ClassInitialization.h"
#include "ext/swing/jxx.ext.swing.ImageIcon.h"
#include "io/jxx.io.ByteArrayInputStream.h"
#include "io/jxx.io.ByteArrayOutputStream.h"
#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"
#include "lang/jxx.lang.String.h"

namespace {

TEST(ImageIconParity, ByteArrayConstructorDecodesJpeg)
{
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        3, 2, ::jxx::awt::image::BufferedImage::TYPE_INT_RGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0xff336699U));
    const auto encoded = ::jxx::ext::imageio::internal::JpegCodec::encode(image, 0.85F);
    auto icon = ::jxx::NEW<::jxx::ext::swing::ImageIcon>(encoded);
    ASSERT_NE(icon, nullptr);
    EXPECT_EQ(icon->getIconWidth(), 3);
    EXPECT_EQ(icon->getIconHeight(), 2);
    EXPECT_NE(icon->getImage(), nullptr);
}

TEST(ImageIconParity, InvalidByteArrayProducesUnloadedIcon)
{
    auto encoded = ::jxx::NEW<::jxx::lang::JxxArray<::jxx::lang::jbyte, 1U>>(4);
    (*encoded)[0] = 1;
    (*encoded)[1] = 2;
    (*encoded)[2] = 3;
    (*encoded)[3] = 4;
    EXPECT_NO_THROW({
        auto icon = ::jxx::NEW<::jxx::ext::swing::ImageIcon>(encoded);
        EXPECT_EQ(icon->getImage(), nullptr);
        EXPECT_EQ(icon->getIconWidth(), -1);
        EXPECT_EQ(icon->getIconHeight(), -1);
    });
}

TEST(ImageIconParity, ClassMetadataIsDistinctAndAvailable)
{
    ::jxx::ext::swing::initializeClasses();
    auto type = ::jxx::ext::swing::ImageIcon::Class();
    ASSERT_NE(type, nullptr);
    ASSERT_NE(type->getName(), nullptr);
    EXPECT_EQ(type->getName()->utf8(), "jxx.ext.swing.ImageIcon");
}

TEST(ImageIconParity, SerializationRoundTripPreservesImageAndDescription)
{
    ::jxx::ext::swing::initializeClasses();
    auto image = ::jxx::NEW<::jxx::awt::image::BufferedImage>(
        2, 2, ::jxx::awt::image::BufferedImage::TYPE_INT_ARGB);
    image->setRGB(0, 0, static_cast<::jxx::lang::jint>(0x7f123456U));
    auto source = ::jxx::NEW<::jxx::ext::swing::ImageIcon>(
        ::jxx::CAST<::jxx::awt::Image>(image),
        ::jxx::NEW<::jxx::lang::String>("cache image"));

    auto bytes = ::jxx::NEW<::jxx::io::ByteArrayOutputStream>();
    {
        auto out = ::jxx::NEW<::jxx::io::ObjectOutputStream>(
            ::jxx::CAST<::jxx::io::OutputStream>(bytes));
        out->writeObject(::jxx::CAST<::jxx::lang::Object>(source));
        out->close();
    }
    auto input = ::jxx::NEW<::jxx::io::ByteArrayInputStream>(bytes->toByteArray());
    auto in = ::jxx::NEW<::jxx::io::ObjectInputStream>(
        ::jxx::CAST<::jxx::io::InputStream>(input));
    auto restored = ::jxx::CAST<::jxx::ext::swing::ImageIcon>(in->readObject());
    ASSERT_NE(restored, nullptr);
    EXPECT_EQ(restored->getIconWidth(), 2);
    EXPECT_EQ(restored->getIconHeight(), 2);
    ASSERT_NE(restored->getDescription(), nullptr);
    EXPECT_EQ(restored->getDescription()->utf8(), "cache image");
    auto restoredImage = ::jxx::CAST<::jxx::awt::image::BufferedImage>(restored->getImage());
    ASSERT_NE(restoredImage, nullptr);
    EXPECT_EQ(static_cast<std::uint32_t>(restoredImage->getRGB(0, 0)), 0x7f123456U);
}

} // namespace
