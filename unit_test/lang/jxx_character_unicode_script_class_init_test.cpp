#include <gtest/gtest.h>

#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.ClassInitialization.h"

TEST(CharacterUnicodeScriptClassInitTest, PublicNestedEnumIsRegistered) {
    ::jxx::lang::initializeClasses();
    const auto type = ::jxx::lang::Character::UnicodeScript::Class();
    ASSERT_NE(nullptr, type);
    EXPECT_TRUE(type->isEnum());
}
