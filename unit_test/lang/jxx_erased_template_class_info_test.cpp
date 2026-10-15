#include <gtest/gtest.h>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.ClassInfo.h"
#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"

namespace {

template <typename T>
class ErasedBox final
    : public ::jxx::lang::ClassBase<ErasedBox<T>, ::jxx::lang::Object> {
};

TEST(ErasedTemplateClassInfo, SpecializationsShareOneClassAny) {
    const auto integerClass = ErasedBox<::jxx::lang::Integer>::Class();
    const auto primitiveClass = ErasedBox<::jxx::lang::jint>::Class();

    ASSERT_NE(integerClass, nullptr);
    ASSERT_NE(primitiveClass, nullptr);
    EXPECT_EQ(integerClass, primitiveClass);
    EXPECT_EQ(integerClass->getName()->utf8().find('<'), std::string::npos);
    EXPECT_EQ(integerClass->getSimpleName()->utf8(), "ErasedBox");
}

TEST(ErasedTemplateClassInfo, NewRegistersSpecializationAutomatically) {
    const auto object = ::jxx::NEW<ErasedBox<::jxx::lang::jlong>>();

    ASSERT_NE(object, nullptr);
    EXPECT_EQ(object->getClass(), ErasedBox<::jxx::lang::Integer>::Class());
}

TEST(ErasedTemplateClassInfo, RawClassRecognizesAllRegisteredSpecializations) {
    const auto integerObject =
        ::jxx::NEW<ErasedBox<::jxx::lang::Integer>>();
    const auto primitiveObject =
        ::jxx::NEW<ErasedBox<::jxx::lang::jint>>();
    const auto rawClass = ErasedBox<::jxx::lang::Integer>::Class();

    EXPECT_TRUE(rawClass->isInstance(integerObject));
    EXPECT_TRUE(rawClass->isInstance(primitiveObject));
}

} // namespace
