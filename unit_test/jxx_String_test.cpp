#include <memory>
#include <string>
#include <type_traits>
#include <gtest/gtest.h>
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx_types.h"
#include "io/jxx.io.Serializable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Comparable.h"


namespace {

    using jxx::lang::ByteArray;
    using jxx::lang::ByteArrayType;
    using jxx::lang::CharArray;
    using jxx::lang::CharArrayType;
    using jxx::lang::CharSequence;
    using jxx::lang::IntArray;
    using jxx::lang::IntArrayType;
    using jxx::lang::Object;
    using jxx::lang::String;
    using jxx::lang::jbool;
    using jxx::lang::jbyte;
    using jxx::lang::jchar;
    using jxx::lang::jdouble;
    using jxx::lang::jfloat;
    using jxx::lang::jint;
    using jxx::lang::jlong;

    static jxx::Ptr<String> S(const char* value) {
        return jxx::NEW<String>(value);
    }

    static jxx::Ptr<String> S(const std::string& value) {
        return jxx::NEW<String>(value);
    }

    static std::string textOf(const jxx::Ptr<String>& value) {
        if (value == nullptr) {
            return {};
        }

        return value->utf8();
    }

    static CharArray makeChars(
        std::initializer_list<jchar> values) {

        auto result = jxx::NEW<CharArrayType>(
            static_cast<CharArrayType::size_type>(
                values.size()));

        jint index = 0;

        for (const auto value : values) {
            (*result)[index++] = value;
        }

        return result;
    }

    static CharArray makeChars(const char16_t* value) {
        if (value == nullptr) {
            return nullptr;
        }

        std::size_t length = 0;

        while (value[length] != u'\0') {
            ++length;
        }

        auto result = jxx::NEW<CharArrayType>(
            static_cast<CharArrayType::size_type>(
                length));

        for (std::size_t i = 0; i < length; ++i) {
            (*result)[static_cast<jint>(i)] =
                static_cast<jchar>(value[i]);
        }

        return result;
    }

    static ByteArray makeBytes(
        std::initializer_list<unsigned int> values) {

        auto result = jxx::NEW<ByteArrayType>(
            static_cast<ByteArrayType::size_type>(
                values.size()));

        jint index = 0;

        for (const auto value : values) {
            (*result)[index++] =
                static_cast<jbyte>(value & 0xFFU);
        }

        return result;
    }

    static IntArray makeCodePoints(
        std::initializer_list<jint> values) {

        auto result = jxx::NEW<IntArrayType>(
            static_cast<IntArrayType::size_type>(
                values.size()));

        jint index = 0;

        for (const auto value : values) {
            (*result)[index++] = value;
        }

        return result;
    }

    /*
     * Type hierarchy
     */

    TEST(StringTest, ImplementsJava8Interfaces) {
        static_assert(
            std::is_base_of_v<Object, String>,
            "String must extend Object");

        static_assert(
            std::is_base_of_v<CharSequence, String>,
            "String must implement CharSequence");

        static_assert(
            std::is_base_of_v<
            jxx::lang::Comparable<String>,
            String>,
            "String must implement Comparable<String>");

        static_assert(
            std::is_base_of_v<
            jxx::io::Serializable,
            String>,
            "String must implement Serializable");

        SUCCEED();
    }

    /*
     * Constructors
     */

    TEST(StringTest, DefaultConstructorCreatesEmptyString) {
        auto value = jxx::NEW<String>();

        ASSERT_NE(value, nullptr);
        EXPECT_EQ(value->length(), 0);
        EXPECT_TRUE(value->isEmpty());
        EXPECT_EQ(value->utf8(), "");
    }

    TEST(StringTest, Utf8CStringConstructorCopiesValue) {
        auto value = jxx::NEW<String>("hello");

        EXPECT_EQ(value->length(), 5);
        EXPECT_FALSE(value->isEmpty());
        EXPECT_EQ(value->utf8(), "hello");
    }

    TEST(StringTest, StdStringConstructorCopiesValue) {
        const std::string source = "network-config";

        auto value = jxx::NEW<String>(source);

        EXPECT_EQ(value->utf8(), source);
        EXPECT_EQ(
            value->length(),
            static_cast<jint>(source.size()));
    }

    TEST(StringTest, Utf16ConstructorCopiesValue) {
        const std::u16string source = u"hello";

        auto value = jxx::NEW<String>(source);

        EXPECT_EQ(value->length(), 5);
        EXPECT_EQ(value->utf16(), source);
        EXPECT_EQ(value->utf8(), "hello");
    }

    TEST(StringTest, CopyConstructorFromStringReference) {
        auto original = S("copied");
        auto copy = jxx::NEW<String>(original);

        ASSERT_NE(copy, nullptr);
        EXPECT_NE(copy.get(), original.get());
        EXPECT_TRUE(copy->equals(original));
        EXPECT_EQ(copy->utf8(), "copied");
    }

    TEST(StringTest, CharArrayConstructorCopiesCharacters) {
        auto chars = makeChars(u"hello");
        auto value = jxx::NEW<String>(chars);

        EXPECT_EQ(value->utf8(), "hello");
    }

    TEST(StringTest, CharArrayRangeConstructorCopiesRange) {
        auto chars = makeChars(u"012345");

        auto value = jxx::NEW<String>(
            chars,
            2,
            3);

        EXPECT_EQ(value->utf8(), "234");
    }

    TEST(StringTest, IntArrayCodePointConstructorSupportsBmpCharacters) {
        auto codePoints = makeCodePoints({
            static_cast<jint>('A'),
            static_cast<jint>('B'),
            static_cast<jint>('C')
            });

        auto value = jxx::NEW<String>(
            codePoints,
            0,
            3);

        EXPECT_EQ(value->utf8(), "ABC");
        EXPECT_EQ(value->length(), 3);
    }

    TEST(StringTest, IntArrayCodePointConstructorSupportsSupplementaryCharacter) {
        auto codePoints = makeCodePoints({
            static_cast<jint>('A'),
            static_cast<jint>(0x1F600),
            static_cast<jint>('B')
            });

        auto value = jxx::NEW<String>(
            codePoints,
            0,
            3);

        /*
         * A = one UTF-16 code unit.
         * U+1F600 = two UTF-16 code units.
         * B = one UTF-16 code unit.
         */
        EXPECT_EQ(value->length(), 4);
        EXPECT_EQ(value->codePointAt(1), 0x1F600);
    }

    TEST(StringTest, ByteArrayConstructorCopiesAsciiBytes) {
        auto bytes = makeBytes({
            'h', 'e', 'l', 'l', 'o'
            });

        auto value = jxx::NEW<String>(bytes);

        EXPECT_EQ(value->utf8(), "hello");
    }

    TEST(StringTest, ByteArrayRangeConstructorCopiesRequestedRange) {
        auto bytes = makeBytes({
            '0', '1', '2', '3', '4', '5'
            });

        auto value = jxx::NEW<String>(
            bytes,
            2,
            3);

        EXPECT_EQ(value->utf8(), "234");
    }

    /*
     * CharSequence operations
     */

    TEST(StringTest, LengthReturnsUtf16CodeUnitCount) {
        auto value = S("hello");

        EXPECT_EQ(value->length(), 5);
    }

    TEST(StringTest, CharAtReturnsRequestedCharacter) {
        auto value = S("hello");

        EXPECT_EQ(
            value->charAt(0),
            static_cast<jchar>(u'h'));

        EXPECT_EQ(
            value->charAt(4),
            static_cast<jchar>(u'o'));
    }

    TEST(StringTest, CharAtRejectsNegativeIndex) {
        auto value = S("abc");

        EXPECT_ANY_THROW(value->charAt(-1));
    }

    TEST(StringTest, CharAtRejectsIndexEqualToLength) {
        auto value = S("abc");

        EXPECT_ANY_THROW(value->charAt(3));
    }

    TEST(StringTest, SubSequenceReturnsRequestedRange) {
        auto value = S("abcdef");

        auto sequence = value->subSequence(1, 4);

        ASSERT_NE(sequence, nullptr);
        EXPECT_EQ(sequence->length(), 3);
        EXPECT_EQ(
            sequence->charAt(0),
            static_cast<jchar>(u'b'));
        EXPECT_EQ(
            sequence->charAt(1),
            static_cast<jchar>(u'c'));
        EXPECT_EQ(
            sequence->charAt(2),
            static_cast<jchar>(u'd'));
    }

    /*
     * Object behavior
     */

    TEST(StringTest, ToStringReturnsEquivalentString) {
        auto value = S("text");

        auto result = value->toString();

        ASSERT_NE(result, nullptr);
        EXPECT_TRUE(result->equals(value));
        EXPECT_EQ(result->utf8(), "text");
    }

    TEST(StringTest, EqualStringsAreEqual) {
        auto left = S("same");
        auto right = S("same");

        EXPECT_TRUE(left->equals(right));
        EXPECT_TRUE(right->equals(left));
    }

    TEST(StringTest, DifferentStringsAreNotEqual) {
        auto left = S("left");
        auto right = S("right");

        EXPECT_FALSE(left->equals(right));
    }

    TEST(StringTest, StringDoesNotEqualNull) {
        auto value = S("text");

        EXPECT_FALSE(value->equals(nullptr));
    }

    TEST(StringTest, EqualStringsHaveSameHashCode) {
        auto left = S("hash-value");
        auto right = S("hash-value");

        EXPECT_EQ(
            left->hashCode(),
            right->hashCode());
    }

    TEST(StringTest, HashCodeMatchesJavaKnownValue) {
        auto value = S("abc");

        /*
         * Java:
         * 'a' * 31^2 + 'b' * 31 + 'c'
         * = 96354
         */
        EXPECT_EQ(value->hashCode(), 96354);
    }

    TEST(StringTest, EmptyStringHashCodeIsZero) {
        auto value = S("");

        EXPECT_EQ(value->hashCode(), 0);
    }

    /*
     * Comparable
     */

    TEST(StringTest, CompareToReturnsZeroForEqualStrings) {
        auto left = S("abc");
        auto right = S("abc");

        EXPECT_EQ(left->compareTo(right), 0);
    }

    TEST(StringTest, CompareToUsesLexicographicOrdering) {
        auto abc = S("abc");
        auto abd = S("abd");

        EXPECT_LT(abc->compareTo(abd), 0);
        EXPECT_GT(abd->compareTo(abc), 0);
    }

    TEST(StringTest, CompareToUsesLengthWhenPrefixMatches) {
        auto shortValue = S("abc");
        auto longValue = S("abcd");

        EXPECT_LT(
            shortValue->compareTo(longValue),
            0);

        EXPECT_GT(
            longValue->compareTo(shortValue),
            0);
    }

    /*
     * Code-point operations
     */

    TEST(StringTest, CodePointAtReturnsBmpCodePoint) {
        auto value = S("ABC");

        EXPECT_EQ(value->codePointAt(1), 'B');
    }

    TEST(StringTest, CodePointBeforeReturnsPreviousCodePoint) {
        auto value = S("ABC");

        EXPECT_EQ(value->codePointBefore(2), 'B');
    }

    TEST(StringTest, CodePointAtCombinesSurrogatePair) {
        const std::u16string source = {
            u'A',
            static_cast<char16_t>(0xD83D),
            static_cast<char16_t>(0xDE00),
            u'B'
        };

        auto value = jxx::NEW<String>(source);

        EXPECT_EQ(value->length(), 4);
        EXPECT_EQ(value->codePointAt(1), 0x1F600);
        EXPECT_EQ(value->codePointBefore(3), 0x1F600);
    }

    TEST(StringTest, CodePointCountCountsSupplementaryAsOne) {
        const std::u16string source = {
            u'A',
            static_cast<char16_t>(0xD83D),
            static_cast<char16_t>(0xDE00),
            u'B'
        };

        auto value = jxx::NEW<String>(source);

        EXPECT_EQ(
            value->codePointCount(
                0,
                value->length()),
            3);
    }

    TEST(StringTest, OffsetByCodePointsHandlesSupplementaryCodePoint) {
        const std::u16string source = {
            u'A',
            static_cast<char16_t>(0xD83D),
            static_cast<char16_t>(0xDE00),
            u'B'
        };

        auto value = jxx::NEW<String>(source);

        EXPECT_EQ(
            value->offsetByCodePoints(0, 1),
            1);

        EXPECT_EQ(
            value->offsetByCodePoints(0, 2),
            3);

        EXPECT_EQ(
            value->offsetByCodePoints(0, 3),
            4);
    }

    /*
     * getChars and arrays
     */

    TEST(StringTest, GetCharsCopiesRequestedRange) {
        auto value = S("abcdef");

        auto destination =
            jxx::NEW<CharArrayType>(8);

        destination->fill(
            static_cast<jchar>(u'_'));

        value->getChars(
            1,
            4,
            destination,
            2);

        EXPECT_EQ(
            (*destination)[0],
            static_cast<jchar>(u'_'));

        EXPECT_EQ(
            (*destination)[1],
            static_cast<jchar>(u'_'));

        EXPECT_EQ(
            (*destination)[2],
            static_cast<jchar>(u'b'));

        EXPECT_EQ(
            (*destination)[3],
            static_cast<jchar>(u'c'));

        EXPECT_EQ(
            (*destination)[4],
            static_cast<jchar>(u'd'));

        EXPECT_EQ(
            (*destination)[5],
            static_cast<jchar>(u'_'));
    }

    TEST(StringTest, ToCharArrayReturnsIndependentCopy) {
        auto value = S("abc");

        auto chars = value->toCharArray();

        ASSERT_NE(chars, nullptr);
        ASSERT_EQ(chars->length, 3U);

        EXPECT_EQ(
            (*chars)[0],
            static_cast<jchar>(u'a'));

        EXPECT_EQ(
            (*chars)[1],
            static_cast<jchar>(u'b'));

        EXPECT_EQ(
            (*chars)[2],
            static_cast<jchar>(u'c'));

        (*chars)[0] = static_cast<jchar>(u'X');

        EXPECT_EQ(value->utf8(), "abc");
    }

    /*
     * Content equality and case-insensitive comparisons
     */

    TEST(StringTest, ContentEqualsCharSequence) {
        auto value = S("hello");

        jxx::Ptr<CharSequence> sequence =
            jxx::CAST<CharSequence>(
                S("hello"));

        EXPECT_TRUE(value->contentEquals(sequence));
    }

    TEST(StringTest, EqualsIgnoreCaseHandlesAsciiCase) {
        auto lower = S("hello");
        auto upper = S("HELLO");

        EXPECT_TRUE(
            lower->equalsIgnoreCase(upper));

        EXPECT_TRUE(
            upper->equalsIgnoreCase(lower));
    }

    TEST(StringTest, EqualsIgnoreCaseDetectsDifferentText) {
        auto left = S("hello");
        auto right = S("world");

        EXPECT_FALSE(
            left->equalsIgnoreCase(right));
    }

    TEST(StringTest, CompareToIgnoreCaseReturnsZeroForAsciiCaseDifference) {
        auto lower = S("network");
        auto upper = S("NETWORK");

        EXPECT_EQ(
            lower->compareToIgnoreCase(upper),
            0);
    }

    /*
     * Prefix, suffix, regions
     */

    TEST(StringTest, StartsWithFindsPrefix) {
        auto value = S("network-config");

        EXPECT_TRUE(value->startsWith(S("network")));
        EXPECT_FALSE(value->startsWith(S("config")));
    }

    TEST(StringTest, StartsWithOffsetFindsPrefixAtOffset) {
        auto value = S("network-config");

        EXPECT_TRUE(
            value->startsWith(
                S("config"),
                8));

        EXPECT_FALSE(
            value->startsWith(
                S("network"),
                1));
    }

    TEST(StringTest, EndsWithFindsSuffix) {
        auto value = S("network-config.json");

        EXPECT_TRUE(value->endsWith(S(".json")));
        EXPECT_FALSE(value->endsWith(S(".xml")));
    }

    TEST(StringTest, RegionMatchesComparesRequestedRegions) {
        auto left = S("012345");
        auto right = S("XX234YY");

        EXPECT_TRUE(
            left->regionMatches(
                2,
                right,
                2,
                3));
    }

    TEST(StringTest, RegionMatchesCanIgnoreCase) {
        auto left = S("HelloWorld");
        auto right = S("xxWORLDyy");

        EXPECT_TRUE(
            left->regionMatches(
                static_cast<jbool>(true),
                5,
                right,
                2,
                5));

        EXPECT_FALSE(
            left->regionMatches(
                static_cast<jbool>(false),
                5,
                right,
                2,
                5));
    }

    /*
     * Character searching
     */

    TEST(StringTest, IndexOfCharacterFindsFirstOccurrence) {
        auto value = S("banana");

        EXPECT_EQ(value->indexOf('a'), 1);
        EXPECT_EQ(value->indexOf('z'), -1);
    }

    TEST(StringTest, IndexOfCharacterHonorsFromIndex) {
        auto value = S("banana");

        EXPECT_EQ(
            value->indexOf('a', 2),
            3);
    }

    TEST(StringTest, LastIndexOfCharacterFindsLastOccurrence) {
        auto value = S("banana");

        EXPECT_EQ(value->lastIndexOf('a'), 5);
        EXPECT_EQ(value->lastIndexOf('z'), -1);
    }

    TEST(StringTest, LastIndexOfCharacterHonorsFromIndex) {
        auto value = S("banana");

        EXPECT_EQ(
            value->lastIndexOf('a', 4),
            3);
    }

    /*
     * String searching
     */

    TEST(StringTest, IndexOfStringFindsFirstOccurrence) {
        auto value = S("one two one");

        EXPECT_EQ(value->indexOf(S("one")), 0);
        EXPECT_EQ(value->indexOf(S("two")), 4);
        EXPECT_EQ(value->indexOf(S("missing")), -1);
    }

    TEST(StringTest, IndexOfStringHonorsFromIndex) {
        auto value = S("one two one");

        EXPECT_EQ(
            value->indexOf(
                S("one"),
                1),
            8);
    }

    TEST(StringTest, LastIndexOfStringFindsLastOccurrence) {
        auto value = S("one two one");

        EXPECT_EQ(
            value->lastIndexOf(S("one")),
            8);

        EXPECT_EQ(
            value->lastIndexOf(S("missing")),
            -1);
    }

    TEST(StringTest, LastIndexOfStringHonorsFromIndex) {
        auto value = S("one two one");

        EXPECT_EQ(
            value->lastIndexOf(
                S("one"),
                7),
            0);
    }

    /*
     * Substring and concatenation
     */

    TEST(StringTest, SubstringFromIndexReturnsSuffix) {
        auto value = S("abcdef");

        auto result = value->substring(2);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "cdef");
    }

    TEST(StringTest, SubstringRangeReturnsRequestedText) {
        auto value = S("abcdef");

        auto result = value->substring(1, 4);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "bcd");
    }

    TEST(StringTest, SubstringRejectsInvalidRange) {
        auto value = S("abc");

        EXPECT_ANY_THROW(value->substring(-1));
        EXPECT_ANY_THROW(value->substring(4));
        EXPECT_ANY_THROW(value->substring(2, 1));
        EXPECT_ANY_THROW(value->substring(0, 4));
    }

    TEST(StringTest, ConcatAppendsString) {
        auto left = S("hello");
        auto right = S(" world");

        auto result = left->concat(right);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello world");

        EXPECT_EQ(left->utf8(), "hello");
        EXPECT_EQ(right->utf8(), " world");
    }

    TEST(StringTest, ConcatEmptyStringPreservesValue) {
        auto value = S("hello");

        auto result = value->concat(S(""));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello");
    }

    /*
     * Replace and contains
     */

    TEST(StringTest, ReplaceCharacterReplacesAllOccurrences) {
        auto value = S("banana");

        auto result = value->replace(
            static_cast<jchar>(u'a'),
            static_cast<jchar>(u'o'));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "bonono");
        EXPECT_EQ(value->utf8(), "banana");
    }

    TEST(StringTest, ContainsFindsCharSequence) {
        auto value = S("network-config");

        jxx::Ptr<CharSequence> found =
            jxx::CAST<CharSequence>(
                S("config"));

        jxx::Ptr<CharSequence> missing =
            jxx::CAST<CharSequence>(
                S("missing"));

        EXPECT_TRUE(value->contains(found));
        EXPECT_FALSE(value->contains(missing));
    }

    /*
     * Regex operations
     */

    TEST(StringTest, MatchesRequiresEntireStringMatch) {
        auto value = S("12345");

        EXPECT_TRUE(value->matches(S("[0-9]+")));
        EXPECT_FALSE(value->matches(S("[a-z]+")));
    }

    TEST(StringTest, ReplaceFirstReplacesOnlyFirstMatch) {
        auto value = S("one one one");

        auto result = value->replaceFirst(
            S("one"),
            S("two"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "two one one");
    }

    TEST(StringTest, ReplaceAllReplacesEveryMatch) {
        auto value = S("one one one");

        auto result = value->replaceAll(
            S("one"),
            S("two"));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "two two two");
    }

    /*
     * Split
     */

    TEST(StringTest, SplitReturnsExpectedElements) {
        auto value = S("one,two,three");

        auto parts = value->split(S(","));

        ASSERT_NE(parts, nullptr);
        ASSERT_EQ(parts->length, 3U);

        ASSERT_NE((*parts)[0], nullptr);
        ASSERT_NE((*parts)[1], nullptr);
        ASSERT_NE((*parts)[2], nullptr);

        EXPECT_EQ((*parts)[0]->utf8(), "one");
        EXPECT_EQ((*parts)[1]->utf8(), "two");
        EXPECT_EQ((*parts)[2]->utf8(), "three");
    }

    TEST(StringTest, SplitWithPositiveLimitRestrictsResultCount) {
        auto value = S("one,two,three");

        auto parts = value->split(S(","), 2);

        ASSERT_NE(parts, nullptr);
        ASSERT_EQ(parts->length, 2U);

        EXPECT_EQ((*parts)[0]->utf8(), "one");
        EXPECT_EQ((*parts)[1]->utf8(), "two,three");
    }

    /*
     * Case conversion and trimming
     */

    TEST(StringTest, ToLowerCaseConvertsAsciiCharacters) {
        auto value = S("JXX String 123");

        auto result = value->toLowerCase();

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "jxx string 123");
    }

    TEST(StringTest, ToUpperCaseConvertsAsciiCharacters) {
        auto value = S("Jxx String 123");

        auto result = value->toUpperCase();

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "JXX STRING 123");
    }

    TEST(StringTest, TrimRemovesLeadingAndTrailingAsciiWhitespace) {
        auto value = S(" \t\r\n hello world \n ");

        auto result = value->trim();

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello world");
    }

    TEST(StringTest, TrimDoesNotRemoveInternalWhitespace) {
        auto value = S("  hello   world  ");

        auto result = value->trim();

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello   world");
    }

    /*
     * Static valueOf
     */

    TEST(StringTest, ValueOfBooleanMatchesJavaText) {
        EXPECT_EQ(
            String::valueOf(
                static_cast<jbool>(true))->utf8(),
            "true");

        EXPECT_EQ(
            String::valueOf(
                static_cast<jbool>(false))->utf8(),
            "false");
    }

    TEST(StringTest, ValueOfCharacterCreatesOneCharacterString) {
        auto result = String::valueOf(
            static_cast<jchar>(u'Z'));

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "Z");
        EXPECT_EQ(result->length(), 1);
    }

    TEST(StringTest, ValueOfIntegerTypes) {
        EXPECT_EQ(
            String::valueOf(
                static_cast<jint>(-42))->utf8(),
            "-42");

        EXPECT_EQ(
            String::valueOf(
                static_cast<jlong>(
                    1234567890LL))->utf8(),
            "1234567890");
    }

    TEST(StringTest, ValueOfFloatingPointTypes) {
        auto floatValue = String::valueOf(
            static_cast<jfloat>(1.5f));

        auto doubleValue = String::valueOf(
            static_cast<jdouble>(2.25));

        ASSERT_NE(floatValue, nullptr);
        ASSERT_NE(doubleValue, nullptr);

        EXPECT_NE(
            floatValue->utf8().find("1.5"),
            std::string::npos);

        EXPECT_NE(
            doubleValue->utf8().find("2.25"),
            std::string::npos);
    }

    TEST(StringTest, ValueOfObjectUsesObjectToString) {
        jxx::Ptr<Object> object =
            jxx::CAST<Object>(S("object-text"));

        auto result = String::valueOf(object);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "object-text");
    }

    TEST(StringTest, ValueOfNullObjectReturnsNullText) {
        jxx::Ptr<Object> object = nullptr;

        auto result = String::valueOf(object);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "null");
    }

    TEST(StringTest, ValueOfCharArrayCopiesArray) {
        auto source = makeChars(u"hello");

        auto result = String::valueOf(source);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello");
    }

    TEST(StringTest, ValueOfCharArrayRangeCopiesRange) {
        auto source = makeChars(u"012345");

        auto result = String::valueOf(
            source,
            2,
            3);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "234");
    }

    TEST(StringTest, CopyValueOfCharArrayCopiesArray) {
        auto source = makeChars(u"hello");

        auto result = String::copyValueOf(source);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "hello");
    }

    TEST(StringTest, CopyValueOfCharArrayRangeCopiesRange) {
        auto source = makeChars(u"012345");

        auto result = String::copyValueOf(
            source,
            1,
            4);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "1234");
    }
    }

    