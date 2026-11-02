#include <gtest/gtest.h>
#include <type_traits>

#include "awt/image/jxx.awt.image.ImageConsumer.h"
#include "awt/image/jxx.awt.image.ImageFilter.h"
#include "lang/jxx.lang.Cloneable.h"
#include "lang/jxx.lang.Object.h"

TEST(ImageFilterInheritanceParity, ExtendsObjectAndImplementsSourceInterfaces) {
    static_assert(std::is_base_of_v<::jxx::lang::Object, ::jxx::awt::image::ImageFilter>);
    static_assert(std::is_base_of_v<::jxx::awt::image::ImageConsumer, ::jxx::awt::image::ImageFilter>);
    static_assert(std::is_base_of_v<::jxx::lang::Cloneable, ::jxx::awt::image::ImageFilter>);
    SUCCEED();
}

TEST(ImageFilterInheritanceParity, PublicTypeHasClassMetadata) {
    EXPECT_NE(nullptr, ::jxx::awt::image::ImageFilter::Class());
}
