#include <gtest/gtest.h>

#include <cstdint>
#include <initializer_list>
#include <string>

#include "lang/jxx.lang.Character.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.IllegalArgumentException.h"
#include "lang/jxx.lang.IndexOutOfBoundsException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"

namespace {

using ::jxx::lang::Character;
using ::jxx::lang::CharArray;
using ::jxx::lang::CharSequence;
using ::jxx::lang::IllegalArgumentException;
using ::jxx::lang::IndexOutOfBoundsException;
using ::jxx::lang::JxxArray;
using ::jxx::lang::NullPointerException;
using ::jxx::lang::String;

constexpr ::jxx::lang::jchar kHigh = static_cast<::jxx::lang::jchar>(0xD83D);
constexpr ::jxx::lang::jchar kLow = static_cast<::jxx::lang::jchar>(0xDE00);
constexpr ::jxx::lang::jint kSupplementary = 0x1F600;

CharArray makeChars(std::initializer_list<::jxx::lang::jchar> values) {
    auto result = ::jxx::NEW<JxxArray<::jxx::lang::jchar, 1U>>(
        static_cast<std::uint32_t>(values.size()));
    ::jxx::lang::jint index = 0;
    for (const auto value : values) {
        (*result)[index++] = value;
    }
    return result;
}

::jxx::Ptr<CharSequence> makeSequence(const std::u16string& value) {
    return ::jxx::NEW<String>(value);
}

TEST(CharacterUtf16TraversalTest, CodePointAtCombinesOnlyPairsInsideTheTraversalLimit) {
    const auto a = makeChars({u'A', kHigh, kLow, u'B'});

    EXPECT_EQ(static_cast<::jxx::lang::jint>(u'A'), Character::codePointAt(a, 0));
    EXPECT_EQ(kSupplementary, Character::codePointAt(a, 1));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(kHigh), Character::codePointAt(a, 1, 2));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(kLow), Character::codePointAt(a, 2));

    const auto seq = makeSequence(std::u16string{u'A', kHigh, kLow, u'B'});
    EXPECT_EQ(kSupplementary, Character::codePointAt(seq, 1));
}

TEST(CharacterUtf16TraversalTest, CodePointBeforeCombinesOnlyPairsInsideTheTraversalStart) {
    const auto a = makeChars({u'A', kHigh, kLow, u'B'});

    EXPECT_EQ(kSupplementary, Character::codePointBefore(a, 3));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(kLow), Character::codePointBefore(a, 3, 2));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(kHigh), Character::codePointBefore(a, 2));

    const auto seq = makeSequence(std::u16string{u'A', kHigh, kLow, u'B'});
    EXPECT_EQ(kSupplementary, Character::codePointBefore(seq, 3));
}

TEST(CharacterUtf16TraversalTest, UnpairedSurrogatesRemainIndividualCodePoints) {
    const auto a = makeChars({kHigh, u'X', kLow});
    const auto seq = makeSequence(std::u16string{kHigh, u'X', kLow});

    EXPECT_EQ(static_cast<::jxx::lang::jint>(kHigh), Character::codePointAt(a, 0));
    EXPECT_EQ(static_cast<::jxx::lang::jint>(kLow), Character::codePointBefore(a, 3));
    EXPECT_EQ(3, Character::codePointCount(a, 0, 3));
    EXPECT_EQ(3, Character::codePointCount(seq, 0, 3));
}

TEST(CharacterUtf16TraversalTest, CodePointCountObservesRequestedUtf16Range) {
    const auto a = makeChars({u'A', kHigh, kLow, u'B', kHigh});
    const auto seq = makeSequence(std::u16string{u'A', kHigh, kLow, u'B', kHigh});

    EXPECT_EQ(4, Character::codePointCount(a, 0, 5));
    EXPECT_EQ(1, Character::codePointCount(a, 1, 2));
    EXPECT_EQ(1, Character::codePointCount(a, 2, 1));
    EXPECT_EQ(0, Character::codePointCount(a, 5, 0));

    EXPECT_EQ(4, Character::codePointCount(seq, 0, 5));
    EXPECT_EQ(1, Character::codePointCount(seq, 1, 3));
    EXPECT_EQ(0, Character::codePointCount(seq, 5, 5));
}

TEST(CharacterUtf16TraversalTest, OffsetByCodePointsMovesAcrossPairsAndUnpairedSurrogates) {
    const auto a = makeChars({u'A', kHigh, kLow, u'B', kHigh});
    const auto seq = makeSequence(std::u16string{u'A', kHigh, kLow, u'B', kHigh});

    EXPECT_EQ(3, Character::offsetByCodePoints(a, 0, 5, 0, 2));
    EXPECT_EQ(1, Character::offsetByCodePoints(a, 0, 5, 3, -1));
    EXPECT_EQ(5, Character::offsetByCodePoints(a, 0, 5, 4, 1));
    EXPECT_EQ(4, Character::offsetByCodePoints(a, 1, 3, 1, 2));

    EXPECT_EQ(3, Character::offsetByCodePoints(seq, 0, 2));
    EXPECT_EQ(1, Character::offsetByCodePoints(seq, 3, -1));
    EXPECT_EQ(5, Character::offsetByCodePoints(seq, 4, 1));
    EXPECT_EQ(3, Character::offsetByCodePoints(seq, 3, 0));
}

TEST(CharacterUtf16TraversalTest, ToCharsWritesBmpAndSupplementaryValuesAtDestinationIndex) {
    const auto dst = makeChars({u'X', u'X', u'X', u'X'});

    EXPECT_EQ(1, Character::toChars(static_cast<::jxx::lang::jint>(u'A'), dst, 1));
    EXPECT_EQ(static_cast<::jxx::lang::jchar>(u'A'), (*dst)[1]);

    EXPECT_EQ(2, Character::toChars(kSupplementary, dst, 2));
    EXPECT_EQ(kHigh, (*dst)[2]);
    EXPECT_EQ(kLow, (*dst)[3]);

    const auto generated = Character::toChars(kSupplementary);
    ASSERT_NE(nullptr, generated);
    ASSERT_EQ(2U, generated->length);
    EXPECT_EQ(kHigh, (*generated)[0]);
    EXPECT_EQ(kLow, (*generated)[1]);
}

TEST(CharacterUtf16TraversalTest, NullInputsThrowNullPointerException) {
    const CharArray nullArray;
    const ::jxx::Ptr<CharSequence> nullSequence;

    EXPECT_THROW(Character::codePointAt(nullArray, 0), NullPointerException);
    EXPECT_THROW(Character::codePointAt(nullSequence, 0), NullPointerException);
    EXPECT_THROW(Character::codePointBefore(nullArray, 1), NullPointerException);
    EXPECT_THROW(Character::codePointBefore(nullSequence, 1), NullPointerException);
    EXPECT_THROW(Character::codePointCount(nullArray, 0, 0), NullPointerException);
    EXPECT_THROW(Character::codePointCount(nullSequence, 0, 0), NullPointerException);
    EXPECT_THROW(Character::offsetByCodePoints(nullArray, 0, 0, 0, 0), NullPointerException);
    EXPECT_THROW(Character::offsetByCodePoints(nullSequence, 0, 0), NullPointerException);
    EXPECT_THROW(Character::toChars(static_cast<::jxx::lang::jint>(u'A'), nullArray, 0),
                 NullPointerException);
}

TEST(CharacterUtf16TraversalTest, InvalidIndexesAndRangesThrowIndexOutOfBoundsException) {
    const auto a = makeChars({u'A', kHigh, kLow});
    const auto seq = makeSequence(std::u16string{u'A', kHigh, kLow});

    EXPECT_THROW(Character::codePointAt(a, -1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointAt(a, 3), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointAt(a, 1, 1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointAt(a, 0, 4), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointBefore(a, 0), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointBefore(a, 4), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointBefore(a, 1, 1), IndexOutOfBoundsException);

    EXPECT_THROW(Character::codePointCount(a, -1, 1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointCount(a, 0, -1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointCount(a, 2, 2), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointCount(seq, 2, 1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::codePointCount(seq, 0, 4), IndexOutOfBoundsException);

    EXPECT_THROW(Character::offsetByCodePoints(a, 0, 3, 0, -1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::offsetByCodePoints(a, 0, 3, 3, 1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::offsetByCodePoints(a, 1, 2, 0, 0), IndexOutOfBoundsException);
    EXPECT_THROW(Character::offsetByCodePoints(seq, 0, -1), IndexOutOfBoundsException);
    EXPECT_THROW(Character::offsetByCodePoints(seq, 3, 1), IndexOutOfBoundsException);

    EXPECT_THROW(Character::toChars(static_cast<::jxx::lang::jint>(u'A'), a, -1),
                 IndexOutOfBoundsException);
    EXPECT_THROW(Character::toChars(kSupplementary, a, 2), IndexOutOfBoundsException);
}

TEST(CharacterUtf16TraversalTest, InvalidCodePointsThrowIllegalArgumentException) {
    const auto dst = makeChars({u'X', u'X'});

    EXPECT_THROW(Character::toChars(-1), IllegalArgumentException);
    EXPECT_THROW(Character::toChars(0x110000), IllegalArgumentException);
    EXPECT_THROW(Character::toChars(-1, dst, 0), IllegalArgumentException);
    EXPECT_THROW(Character::toChars(0x110000, dst, 0), IllegalArgumentException);
}

} // namespace
