#include <gtest/gtest.h>

#include <type_traits>

#include "lang/jxx.lang.Class.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.Void.h"

TEST(JxxLangVoidStage, IsFinalAndNotConstructible) {
    EXPECT_TRUE((std::is_final_v<::jxx::lang::Void>));
    EXPECT_FALSE((std::is_default_constructible_v<::jxx::lang::Void>));
    EXPECT_FALSE((std::is_copy_constructible_v<::jxx::lang::Void>));
}

TEST(JxxLangVoidStage, TypeRepresentsPrimitiveVoid) {
    ASSERT_NE(::jxx::lang::Void::TYPE, nullptr);
    EXPECT_EQ(::jxx::lang::Void::TYPE->getName()->utf8(), "void");
    EXPECT_TRUE(::jxx::lang::Void::TYPE->isPrimitive());
    EXPECT_FALSE(::jxx::lang::Void::TYPE->isArray());
    EXPECT_FALSE(::jxx::lang::Void::TYPE->isInterface());
}

TEST(JxxLangVoidStage, TypeIsRegisteredByName) {
    const auto type = ::jxx::lang::ClassAny::forName(
        ::jxx::NEW<::jxx::lang::String>("void"));
    EXPECT_EQ(type, ::jxx::lang::Void::TYPE);
}
