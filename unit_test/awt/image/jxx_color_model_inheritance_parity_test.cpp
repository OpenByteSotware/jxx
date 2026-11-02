#include <gtest/gtest.h>
#include <type_traits>

#include "awt/image/jxx.awt.image.ColorModel.h"
#include "awt/image/jxx.awt.image.DirectColorModel.h"
#include "awt/image/jxx.awt.image.PackedColorModel.h"
#include "awt/jxx.awt.Transparency.h"

TEST(ColorModelInheritanceParity, ColorModelImplementsTransparency) {
    static_assert(std::is_base_of_v<::jxx::awt::Transparency,
        ::jxx::awt::image::ColorModel>);
    SUCCEED();
}

TEST(ColorModelInheritanceParity, DirectColorModelUsesPackedColorModelSuperclass) {
    static_assert(std::is_base_of_v<::jxx::awt::image::PackedColorModel,
        ::jxx::awt::image::DirectColorModel>);
    EXPECT_TRUE((std::is_base_of_v<::jxx::awt::image::ColorModel,
        ::jxx::awt::image::DirectColorModel>));
}

TEST(ColorModelInheritanceParity, PackageTypesExposeClassMetadata) {
    EXPECT_NE(nullptr, ::jxx::awt::image::ColorModel::Class());
    EXPECT_NE(nullptr, ::jxx::awt::image::PackedColorModel::Class());
    EXPECT_NE(nullptr, ::jxx::awt::image::DirectColorModel::Class());
}

TEST(ColorModelInheritanceParity, DirectMasksAndTransparencyRemainOperational) {
    auto model = ::jxx::NEW<::jxx::awt::image::DirectColorModel>(
        32, 0x00ff0000, 0x0000ff00, 0x000000ff,
        static_cast<::jxx::lang::jint>(0xff000000U));
    EXPECT_EQ(0x00ff0000, model->getRedMask());
    EXPECT_EQ(0x0000ff00, model->getGreenMask());
    EXPECT_EQ(0x000000ff, model->getBlueMask());
    EXPECT_EQ(static_cast<::jxx::lang::jint>(0xff000000U), model->getAlphaMask());
    EXPECT_EQ(::jxx::awt::Transparency::TRANSLUCENT, model->getTransparency());
}
