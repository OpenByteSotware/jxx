#include <gtest/gtest.h>

#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"

namespace {
using jxx::lang::Character;
using jxx::lang::String;

TEST(CharacterUnicodeScriptTest, ValuesExposeCompleteJava8SetInDeclarationOrder) {
    const auto values = Character::UnicodeScript::values();
    ASSERT_NE(values, nullptr);
    ASSERT_EQ(values->length, 102U);
    EXPECT_EQ((*values)[0].get(), Character::UnicodeScript::COMMON.get());
    EXPECT_EQ((*values)[1].get(), Character::UnicodeScript::LATIN.get());
    EXPECT_EQ((*values)[40].get(), Character::UnicodeScript::INHERITED.get());
    EXPECT_EQ((*values)[101].get(), Character::UnicodeScript::UNKNOWN.get());
    EXPECT_EQ(Character::UnicodeScript::UNKNOWN->ordinal(), 101);
}

TEST(CharacterUnicodeScriptTest, ValueOfUsesExactEnumNames) {
    EXPECT_EQ(Character::UnicodeScript::valueOf(jxx::NEW<String>("LATIN")).get(),
              Character::UnicodeScript::LATIN.get());
    EXPECT_THROW(Character::UnicodeScript::valueOf(jxx::NEW<String>("latin")),
                 jxx::lang::IllegalArgumentException);
}

TEST(CharacterUnicodeScriptTest, ForNameAcceptsNormalizedLongNames) {
    EXPECT_EQ(Character::UnicodeScript::forName(
                  jxx::NEW<String>("old-south-arabian")).get(),
              Character::UnicodeScript::OLD_SOUTH_ARABIAN.get());
    EXPECT_EQ(Character::UnicodeScript::forName(
                  jxx::NEW<String>("Canadian Aboriginal")).get(),
              Character::UnicodeScript::CANADIAN_ABORIGINAL.get());
    EXPECT_EQ(Character::UnicodeScript::forName(jxx::NEW<String>("Latn")).get(),
              Character::UnicodeScript::LATIN.get());
    EXPECT_EQ(Character::UnicodeScript::forName(jxx::NEW<String>("Zinh")).get(),
              Character::UnicodeScript::INHERITED.get());
}

TEST(CharacterUnicodeScriptTest, OfUsesUnicodeUcdScriptProperties) {
    EXPECT_EQ(Character::UnicodeScript::of(0x0041).get(),
              Character::UnicodeScript::LATIN.get());
    EXPECT_EQ(Character::UnicodeScript::of(0x03A9).get(),
              Character::UnicodeScript::GREEK.get());
    EXPECT_EQ(Character::UnicodeScript::of(0x0416).get(),
              Character::UnicodeScript::CYRILLIC.get());
    EXPECT_EQ(Character::UnicodeScript::of(0x4E00).get(),
              Character::UnicodeScript::HAN.get());
    EXPECT_EQ(Character::UnicodeScript::of(0x10400).get(),
              Character::UnicodeScript::DESERET.get());
    EXPECT_THROW(Character::UnicodeScript::of(0x110000),
                 jxx::lang::IllegalArgumentException);
}

TEST(CharacterUnicodeBackingTest, GeneralCategoryQueriesUseUnicodeData) {
    EXPECT_TRUE(Character::isUpperCase(0x00C4));
    EXPECT_TRUE(Character::isLowerCase(0x00E4));
    EXPECT_TRUE(Character::isLetter(0x03A9));
    EXPECT_TRUE(Character::isDigit(0x0665));
    EXPECT_EQ(Character::getType(0x0301), Character::NON_SPACING_MARK);
    EXPECT_TRUE(Character::isMirrored(0x0028));
}

TEST(CharacterUnicodeBackingTest, SimpleCaseMappingsPreserveSingleCodePointContract) {
    EXPECT_EQ(Character::toLowerCase(0x00C4), 0x00E4);
    EXPECT_EQ(Character::toUpperCase(0x00E4), 0x00C4);
    EXPECT_EQ(Character::toLowerCase(0x03A3), 0x03C3);
    EXPECT_EQ(Character::toUpperCase(0x03C3), 0x03A3);
    EXPECT_EQ(Character::toUpperCase(0x00DF), 0x00DF);
}

TEST(CharacterUnicodeBackingTest, SupplementaryCodePointsRemainSupported) {
    EXPECT_TRUE(Character::isValidCodePoint(0x10400));
    EXPECT_TRUE(Character::isSupplementaryCodePoint(0x10400));
    EXPECT_TRUE(Character::isLetter(0x10400));
    EXPECT_EQ(Character::toLowerCase(0x10400), 0x10428);
}
} // namespace
