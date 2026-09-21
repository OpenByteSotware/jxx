#include <gtest/gtest.h>
#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.String.h"
namespace { using jxx::lang::Character; using jxx::lang::String;
TEST(CharacterUnicodeBlockTest, MapsBmpAndSupplementaryBlocks) {
 EXPECT_EQ(Character::UnicodeBlock::of(0x0041).get(), Character::UnicodeBlock::BASIC_LATIN.get());
 EXPECT_EQ(Character::UnicodeBlock::of(0x03A9).get(), Character::UnicodeBlock::GREEK.get());
 EXPECT_EQ(Character::UnicodeBlock::of(0x4E00).get(), Character::UnicodeBlock::CJK_UNIFIED_IDEOGRAPHS.get());
 EXPECT_EQ(Character::UnicodeBlock::of(0x10400).get(), Character::UnicodeBlock::DESERET.get());
 EXPECT_EQ(Character::UnicodeBlock::of(0x1EE00).get(), Character::UnicodeBlock::ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS.get());
}
TEST(CharacterUnicodeBlockTest, ReturnsNullForGapsAndRejectsInvalidValues) {
 EXPECT_EQ(Character::UnicodeBlock::of(0x0870), nullptr);
 EXPECT_EQ(Character::UnicodeBlock::of(0x10200), nullptr);
 EXPECT_THROW(Character::UnicodeBlock::of(-1), jxx::lang::IllegalArgumentException);
 EXPECT_THROW(Character::UnicodeBlock::of(0x110000), jxx::lang::IllegalArgumentException);
}
TEST(CharacterUnicodeBlockTest, CharOverloadUsesUtf16CodeUnit) {
 EXPECT_EQ(Character::UnicodeBlock::of(static_cast<jxx::lang::jchar>(0xD800)).get(), Character::UnicodeBlock::HIGH_SURROGATES.get());
}
TEST(CharacterUnicodeBlockTest, ForNameIgnoresCaseWhitespaceHyphensAndUnderbars) {
 EXPECT_EQ(Character::UnicodeBlock::forName(jxx::NEW<String>("basic latin")).get(), Character::UnicodeBlock::BASIC_LATIN.get());
 EXPECT_EQ(Character::UnicodeBlock::forName(jxx::NEW<String>("Latin-1_Supplement")).get(), Character::UnicodeBlock::LATIN_1_SUPPLEMENT.get());
 EXPECT_EQ(Character::UnicodeBlock::forName(jxx::NEW<String>("ArabicMathematicalAlphabeticSymbols")).get(), Character::UnicodeBlock::ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS.get());
 EXPECT_EQ(Character::UnicodeBlock::forName(jxx::NEW<String>("Greek and Coptic")).get(), Character::UnicodeBlock::GREEK.get());
 EXPECT_EQ(Character::UnicodeBlock::forName(jxx::NEW<String>("Cyrillic Supplement")).get(), Character::UnicodeBlock::CYRILLIC_SUPPLEMENTARY.get());
}
TEST(CharacterUnicodeBlockTest, HistoricalConstantsRemainAvailable) {
 EXPECT_EQ(Character::UnicodeBlock::of(0x20D0).get(), Character::UnicodeBlock::COMBINING_MARKS_FOR_SYMBOLS.get());
 EXPECT_EQ(Character::UnicodeBlock::of(0x0500).get(), Character::UnicodeBlock::CYRILLIC_SUPPLEMENTARY.get());
 EXPECT_NE(Character::UnicodeBlock::SURROGATES_AREA, nullptr);
}
} // namespace
