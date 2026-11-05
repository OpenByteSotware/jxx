#include <gtest/gtest.h>
#include "lang/jxx.lang.Character.h"

TEST(CharacterJava8Parity, IdentifierAndAlphabeticMethodsAreAvailable) {
    EXPECT_TRUE(::jxx::lang::Character::isAlphabetic('A'));
    EXPECT_TRUE(::jxx::lang::Character::isUnicodeIdentifierStart('_'));
    EXPECT_TRUE(::jxx::lang::Character::isUnicodeIdentifierPart('7'));
    EXPECT_TRUE(::jxx::lang::Character::isIdeographic(0x4E00));
    EXPECT_TRUE(::jxx::lang::Character::isIdentifierIgnorable(0x0007));
}
