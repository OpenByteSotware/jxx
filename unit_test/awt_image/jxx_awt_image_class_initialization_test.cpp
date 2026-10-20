#include <gtest/gtest.h>

#include "awt/image/jxx.awt.image.ClassInitialization.h"
#include "awt/image/jxx.awt.image.BufferedImage.h"
#include "awt/image/jxx.awt.image.ColorModel.h"
#include "awt/image/jxx.awt.image.DirectColorModel.h"
#include "awt/image/jxx.awt.image.ImageFilter.h"
#include "awt/image/jxx.awt.image.RGBImageFilter.h"
#include "lang/jxx.lang.String.h"

namespace
{
    template <typename T>
    void expectClassMetadata()
    {
        const auto type = T::Class();
        ASSERT_NE(type, nullptr);
        ASSERT_NE(type->getName(), nullptr);
        EXPECT_FALSE(type->getName()->utf8().empty());
    }

    TEST(AwtImageClassInitialization, RegistersExistingImageClasses)
    {
        ::jxx::awt::image::initializeClasses();
        expectClassMetadata<::jxx::awt::image::BufferedImage>();
        expectClassMetadata<::jxx::awt::image::ColorModel>();
        expectClassMetadata<::jxx::awt::image::DirectColorModel>();
        expectClassMetadata<::jxx::awt::image::ImageFilter>();
        expectClassMetadata<::jxx::awt::image::RGBImageFilter>();
    }
}
