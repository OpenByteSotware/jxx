#include <gtest/gtest.h>

#include <string>

#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringIndexOutOfBoundsException.h"

namespace {

using ::jxx::lang::String;
using ::jxx::lang::StringIndexOutOfBoundsException;

TEST(StringIndexingParityTest, Utf8InputIsStoredAndReturnedWithoutLoss)
{
    const std::string utf8 = u8"Grüße 🌍";
    const auto value = ::jxx::NEW<String>(utf8);

    ASSERT_NE(nullptr, value);
    EXPECT_EQ(utf8, value->utf8());
}

TEST(StringIndexingParityTest, LengthCountsUtf16CodeUnits)
{
    const auto value = ::jxx::NEW<String>(
        std::string(u8"Grüße 🌍"));

    // G r ü ß e space are six UTF-16 code units. The globe is a
    // surrogate pair and therefore occupies two more code units.
    EXPECT_EQ(8, value->length());
}

TEST(StringIndexingParityTest, SubstringUsesUtf16CodeUnitIndices)
{
    const auto value = ::jxx::NEW<String>(
        std::string(u8"Grüße 🌍"));

    const auto left = value->substring(0, 5);
    const auto earth = value->substring(6, 8);

    ASSERT_NE(nullptr, left);
    ASSERT_NE(nullptr, earth);
    EXPECT_EQ(std::string(u8"Grüße"), left->utf8());
    EXPECT_EQ(std::string(u8"🌍"), earth->utf8());
}

TEST(StringIndexingParityTest, CharAtReturnsUtf16SurrogateCodeUnits)
{
    const auto value = ::jxx::NEW<String>(
        std::string(u8"Grüße 🌍"));

    EXPECT_EQ(static_cast<::jxx::lang::jchar>(0xD83C), value->charAt(6));
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(0xDF0D), value->charAt(7));
}

TEST(StringIndexingParityTest, Utf8ByteOffsetsAreNotSubstringIndices)
{
    const std::string utf8 = u8"Grüße 🌍";
    const auto value = ::jxx::NEW<String>(utf8);

    const auto globe = std::string(u8"🌍");
    const auto globeByteOffset = utf8.find(globe);
    ASSERT_NE(std::string::npos, globeByteOffset);
    ASSERT_GT(globeByteOffset + globe.size(),
              static_cast<std::size_t>(value->length()));

    EXPECT_THROW(
        value->substring(
            static_cast<::jxx::lang::jint>(globeByteOffset),
            static_cast<::jxx::lang::jint>(
                globeByteOffset + globe.size())),
        StringIndexOutOfBoundsException);
}

TEST(StringIndexingParityTest, BmpMultibyteUtf8CharacterUsesOneUtf16Index)
{
    const auto value = ::jxx::NEW<String>(std::string(u8"AüB"));

    EXPECT_EQ(3, value->length());
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(0x00FC), value->charAt(1));

    const auto umlaut = value->substring(1, 2);
    ASSERT_NE(nullptr, umlaut);
    EXPECT_EQ(std::string(u8"ü"), umlaut->utf8());
}

TEST(StringIndexingParityTest, SupplementaryCharacterUsesTwoUtf16Indices)
{
    const auto value = ::jxx::NEW<String>(std::string(u8"A🌍B"));

    EXPECT_EQ(4, value->length());

    const auto earth = value->substring(1, 3);
    ASSERT_NE(nullptr, earth);
    EXPECT_EQ(std::string(u8"🌍"), earth->utf8());
}

TEST(StringIndexingParityTest, CodePointAtCombinesSurrogatePair)
{
    const auto value = ::jxx::NEW<String>(std::string(u8"A🌍B"));

    EXPECT_EQ(static_cast<::jxx::lang::jint>(0x1F30D),
              value->codePointAt(1));
}

} // namespace
