#include <memory>
#include <string>
#include <type_traits>
#include <gtest/gtest.h>
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Appendable.h"
#include "lang/jxx.lang.CharSequence.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.String.h"
#include "lang/jxx.lang.StringBuilder.h"
#include "lang/jxx.lang.buildin_array.h"
#include "lang/jxx.lang.NegativeArraySizeException.h"
namespace {

    using jxx::lang::CharArray;
    using jxx::lang::CharArrayType;
    using jxx::lang::CharSequence;
    using jxx::lang::String;
    using jxx::lang::StringBuilder;
    using jxx::lang::jbool;
    using jxx::lang::jchar;
    using jxx::lang::jdouble;
    using jxx::lang::jfloat;
    using jxx::lang::jint;
    using jxx::lang::jlong;

    static jxx::Ptr<String> S(const char* value) {
        return jxx::NEW<String>(std::string(value));
    }

    static jxx::Ptr<String> S(const std::string& value) {
        return jxx::NEW<String>(value);
    }

    static std::string textOf(
        const jxx::Ptr<StringBuilder>& builder) {

        if (builder == nullptr) {
            return {};
        }

        auto value = builder->toString();

        if (value == nullptr) {
            return {};
        }

        return value->utf8();
    }

    static CharArray chars(
        std::initializer_list<jchar> values) {

        auto result =
            jxx::NEW<CharArrayType>(
                static_cast<CharArrayType::size_type>(
                    values.size()));

        jint index = 0;

        for (const auto value : values) {
            (*result)[index++] = value;
        }

        return result;
    }

    static CharArray chars(const char16_t* value) {
        if (value == nullptr) {
            return nullptr;
        }

        std::size_t length = 0;

        while (value[length] != u'\0') {
            ++length;
        }

        auto result =
            jxx::NEW<CharArrayType>(
                static_cast<CharArrayType::size_type>(
                    length));

        for (std::size_t i = 0; i < length; ++i) {
            (*result)[static_cast<jint>(i)] =
                static_cast<jchar>(value[i]);
        }

        return result;
    }

    /*
     * Constructors
     */

    TEST(StringBuilderTest, DefaultConstructorCreatesEmptyBuilder) {
        auto builder = jxx::NEW<StringBuilder>();

        ASSERT_NE(builder, nullptr);
        EXPECT_EQ(builder->length(), 0);
        EXPECT_TRUE(textOf(builder).empty());

        // Java 8 StringBuilder default capacity is 16.
        EXPECT_GE(builder->capacity(), 16);
    }

    TEST(StringBuilderTest, CapacityConstructorUsesRequestedCapacity) {
        auto builder = jxx::NEW<StringBuilder>(32);

        EXPECT_EQ(builder->length(), 0);
        EXPECT_GE(builder->capacity(), 32);
    }

    TEST(StringBuilderTest, NegativeCapacityThrows) {
        EXPECT_THROW(
            jxx::NEW<StringBuilder>(-1),
            jxx::lang::NegativeArraySizeException);
    }

    TEST(StringBuilderTest, StringConstructorCopiesString) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("hello"));

        EXPECT_EQ(builder->length(), 5);
        EXPECT_EQ(textOf(builder), "hello");

        // Java capacity is string length plus 16.
        EXPECT_GE(builder->capacity(), 21);
    }

    TEST(StringBuilderTest, CharSequenceConstructorCopiesSequence) {
        jxx::Ptr<CharSequence> sequence =
            jxx::CAST<CharSequence>(
                S("sequence"));

        auto builder =
            jxx::NEW<StringBuilder>(
                sequence);

        EXPECT_EQ(builder->length(), 8);
        EXPECT_EQ(textOf(builder), "sequence");
    }

    /*
     * Append operations
     */

    TEST(StringBuilderTest, AppendString) {
        auto builder = jxx::NEW<StringBuilder>();

        auto returned = builder->append(S("hello"));

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "hello");
    }

    TEST(StringBuilderTest, AppendNullStringUsesJavaNullText) {
        auto builder = jxx::NEW<StringBuilder>();

        jxx::Ptr<String> nullString = nullptr;
        builder->append(nullString);

        EXPECT_EQ(textOf(builder), "null");
    }

    TEST(StringBuilderTest, AppendBooleanValues) {
        auto builder = jxx::NEW<StringBuilder>();

        builder->append(static_cast<jbool>(true));
        builder->append(S(","));
        builder->append(static_cast<jbool>(false));

        EXPECT_EQ(textOf(builder), "true,false");
    }

    TEST(StringBuilderTest, AppendCharacter) {
        auto builder = jxx::NEW<StringBuilder>();

        auto appendable =
            builder->append(
                static_cast<jchar>(u'A'));

        ASSERT_NE(appendable, nullptr);
        EXPECT_EQ(textOf(builder), "A");
    }

    TEST(StringBuilderTest, AppendSBCharacterReturnsBuilder) {
        auto builder = jxx::NEW<StringBuilder>();

        auto returned =
            builder->appendSB(
                static_cast<jchar>(u'X'));

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "X");
    }

    TEST(StringBuilderTest, AppendIntegerTypes) {
        auto builder = jxx::NEW<StringBuilder>();

        builder->append(static_cast<jint>(-42));
        builder->append(S(","));
        builder->append(static_cast<jlong>(1234567890LL));

        EXPECT_EQ(
            textOf(builder),
            "-42,1234567890");
    }

    TEST(StringBuilderTest, AppendFloatingPointTypes) {
        auto builder = jxx::NEW<StringBuilder>();

        builder->append(static_cast<jfloat>(1.5f));
        builder->append(S(","));
        builder->append(static_cast<jdouble>(2.25));

        const std::string result = textOf(builder);

        EXPECT_NE(result.find("1.5"), std::string::npos);
        EXPECT_NE(result.find("2.25"), std::string::npos);
    }

    TEST(StringBuilderTest, AppendCharArray) {
        auto builder = jxx::NEW<StringBuilder>();
        auto value = chars(u"hello");

        builder->append(value);

        EXPECT_EQ(textOf(builder), "hello");
    }

    TEST(StringBuilderTest, AppendCharArrayRange) {
        auto builder = jxx::NEW<StringBuilder>();
        auto value = chars(u"012345");

        builder->append(value, 2, 3);

        EXPECT_EQ(textOf(builder), "234");
    }

    TEST(StringBuilderTest, AppendCharSequenceRange) {
        auto builder = jxx::NEW<StringBuilder>();

        jxx::Ptr<CharSequence> sequence =
            jxx::CAST<CharSequence>(
                S("012345"));

        builder->appendSB(sequence, 1, 5);

        EXPECT_EQ(textOf(builder), "1234");
    }

    TEST(StringBuilderTest, RepeatedAppendReturnsSameBuilder) {
        auto builder = jxx::NEW<StringBuilder>();

        auto first = builder->append(S("a"));
        auto second = builder->append(S("b"));
        auto third = builder->append(static_cast<jint>(3));

        EXPECT_EQ(first.get(), builder.get());
        EXPECT_EQ(second.get(), builder.get());
        EXPECT_EQ(third.get(), builder.get());
        EXPECT_EQ(textOf(builder), "ab3");
    }

    /*
     * Length and capacity
     */

    TEST(StringBuilderTest, EnsureCapacityDoesNotChangeContent) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("text"));

        const jint oldLength = builder->length();

        builder->ensureCapacity(100);

        EXPECT_GE(builder->capacity(), 100);
        EXPECT_EQ(builder->length(), oldLength);
        EXPECT_EQ(textOf(builder), "text");
    }

    TEST(StringBuilderTest, EnsureCapacityDoesNotShrink) {
        auto builder = jxx::NEW<StringBuilder>(64);

        const jint oldCapacity = builder->capacity();

        builder->ensureCapacity(8);

        EXPECT_EQ(builder->capacity(), oldCapacity);
    }

    TEST(StringBuilderTest, TrimToSizeRetainsContent) {
        auto builder = jxx::NEW<StringBuilder>(128);

        builder->append(S("short"));
        builder->trimToSize();

        EXPECT_EQ(textOf(builder), "short");
        EXPECT_EQ(builder->length(), 5);
        EXPECT_GE(builder->capacity(), builder->length());
    }

    TEST(StringBuilderTest, SetLengthCanTruncateBuilder) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        builder->setLength(3);

        EXPECT_EQ(builder->length(), 3);
        EXPECT_EQ(textOf(builder), "abc");
    }

    TEST(StringBuilderTest, SetLengthCanExtendWithNullCharacters) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abc"));

        builder->setLength(5);

        EXPECT_EQ(builder->length(), 5);
        EXPECT_EQ(builder->charAt(0), static_cast<jchar>(u'a'));
        EXPECT_EQ(builder->charAt(1), static_cast<jchar>(u'b'));
        EXPECT_EQ(builder->charAt(2), static_cast<jchar>(u'c'));
        EXPECT_EQ(builder->charAt(3), static_cast<jchar>(u'\0'));
        EXPECT_EQ(builder->charAt(4), static_cast<jchar>(u'\0'));
    }

    TEST(StringBuilderTest, NegativeSetLengthThrows) {
        auto builder = jxx::NEW<StringBuilder>();

        EXPECT_ANY_THROW(
            builder->setLength(-1));
    }

    /*
     * Character access
     */

    TEST(StringBuilderTest, CharAtReturnsExpectedCharacters) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("hello"));

        EXPECT_EQ(
            builder->charAt(0),
            static_cast<jchar>(u'h'));

        EXPECT_EQ(
            builder->charAt(4),
            static_cast<jchar>(u'o'));
    }

    TEST(StringBuilderTest, CharAtRejectsInvalidIndexes) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abc"));

        EXPECT_ANY_THROW(builder->charAt(-1));
        EXPECT_ANY_THROW(builder->charAt(3));
    }

    TEST(StringBuilderTest, SetCharAtReplacesCharacter) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("hello"));

        builder->setCharAt(
            1,
            static_cast<jchar>(u'a'));

        EXPECT_EQ(textOf(builder), "hallo");
    }

    TEST(StringBuilderTest, SetCharAtRejectsInvalidIndexes) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abc"));

        EXPECT_ANY_THROW(
            builder->setCharAt(
                -1,
                static_cast<jchar>(u'x')));

        EXPECT_ANY_THROW(
            builder->setCharAt(
                3,
                static_cast<jchar>(u'x')));
    }

    /*
     * Delete, replace, and reverse
     */

    TEST(StringBuilderTest, DeleteRemovesSpecifiedRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("0123456789"));

        auto returned =
            builder->delete_(2, 7);

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "01789");
    }

    TEST(StringBuilderTest, DeleteAllowsEndBeyondLength) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        builder->delete_(3, 100);

        EXPECT_EQ(textOf(builder), "abc");
    }

    TEST(StringBuilderTest, DeleteCharAtRemovesOneCharacter) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcd"));

        auto returned =
            builder->deleteCharAt(1);

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "acd");
    }

    TEST(StringBuilderTest, ReplaceChangesSpecifiedRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("hello world"));

        auto returned =
            builder->replace(
                6,
                11,
                S("JXX"));

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "hello JXX");
    }

    TEST(StringBuilderTest, ReverseReversesCharacters) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto returned = builder->reverse();

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "fedcba");
    }

    /*
     * Insert operations
     */

    TEST(StringBuilderTest, InsertString) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ac"));

        auto returned =
            builder->insert(
                1,
                S("b"));

        EXPECT_EQ(returned.get(), builder.get());
        EXPECT_EQ(textOf(builder), "abc");
    }

    TEST(StringBuilderTest, InsertNullStringUsesJavaNullText) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ab"));

        jxx::Ptr<String> nullString = nullptr;

        builder->insert(
            1,
            nullString);

        EXPECT_EQ(textOf(builder), "anullb");
    }

    TEST(StringBuilderTest, InsertCharacter) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ac"));

        builder->insert(
            1,
            static_cast<jchar>(u'b'));

        EXPECT_EQ(textOf(builder), "abc");
    }

    TEST(StringBuilderTest, InsertBoolean) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("value="));

        builder->insert(
            builder->length(),
            static_cast<jbool>(true));

        EXPECT_EQ(textOf(builder), "value=true");
    }

    TEST(StringBuilderTest, InsertInteger) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("AB"));

        builder->insert(
            1,
            static_cast<jint>(123));

        EXPECT_EQ(textOf(builder), "A123B");
    }

    TEST(StringBuilderTest, InsertCharArray) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ad"));

        auto value = chars(u"bc");

        builder->insert(1, value);

        EXPECT_EQ(textOf(builder), "abcd");
    }

    TEST(StringBuilderTest, InsertCharArrayRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ad"));

        auto value = chars(u"012345");

        builder->insert(
            1,
            value,
            2,
            2);

        EXPECT_EQ(textOf(builder), "a23d");
    }

    TEST(StringBuilderTest, InsertCharSequenceRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("ad"));

        jxx::Ptr<CharSequence> sequence =
            jxx::CAST<CharSequence>(
                S("012345"));

        builder->insert(
            1,
            sequence,
            2,
            4);

        EXPECT_EQ(textOf(builder), "a23d");
    }

    TEST(StringBuilderTest, InsertRejectsInvalidOffset) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abc"));

        EXPECT_ANY_THROW(
            builder->insert(
                -1,
                S("x")));

        EXPECT_ANY_THROW(
            builder->insert(
                4,
                S("x")));
    }

    /*
     * getChars
     */

    TEST(StringBuilderTest, GetCharsCopiesRequestedRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto destination =
            jxx::NEW<CharArrayType>(8);

        destination->fill(
            static_cast<jchar>(u'_'));

        builder->getChars(
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

    TEST(StringBuilderTest, GetCharsRejectsInvalidSourceRange) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abc"));

        auto destination =
            jxx::NEW<CharArrayType>(3);

        EXPECT_ANY_THROW(
            builder->getChars(
                -1,
                2,
                destination,
                0));

        EXPECT_ANY_THROW(
            builder->getChars(
                2,
                1,
                destination,
                0));

        EXPECT_ANY_THROW(
            builder->getChars(
                0,
                4,
                destination,
                0));
    }

    TEST(StringBuilderTest, GetCharsRejectsDestinationOverflow) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto destination =
            jxx::NEW<CharArrayType>(2);

        EXPECT_ANY_THROW(
            builder->getChars(
                0,
                3,
                destination,
                0));
    }

    /*
     * Search functions
     */

    TEST(StringBuilderTest, IndexOfFindsFirstOccurrence) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("one two one"));

        EXPECT_EQ(
            builder->indexOf(S("one")),
            0);

        EXPECT_EQ(
            builder->indexOf(S("two")),
            4);

        EXPECT_EQ(
            builder->indexOf(S("missing")),
            -1);
    }

    TEST(StringBuilderTest, IndexOfHonorsFromIndex) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("one two one"));

        EXPECT_EQ(
            builder->indexOf(
                S("one"),
                1),
            8);
    }

    TEST(StringBuilderTest, LastIndexOfFindsLastOccurrence) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("one two one"));

        EXPECT_EQ(
            builder->lastIndexOf(
                S("one")),
            8);

        EXPECT_EQ(
            builder->lastIndexOf(
                S("missing")),
            -1);
    }

    TEST(StringBuilderTest, LastIndexOfHonorsFromIndex) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("one two one"));

        EXPECT_EQ(
            builder->lastIndexOf(
                S("one"),
                7),
            0);
    }

    /*
     * Substring and CharSequence behavior
     */

    TEST(StringBuilderTest, SubstringFromIndexReturnsSuffix) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto result =
            builder->substring(2);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "cdef");
    }

    TEST(StringBuilderTest, SubstringRangeReturnsExpectedText) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto result =
            builder->substring(
                1,
                4);

        ASSERT_NE(result, nullptr);
        EXPECT_EQ(result->utf8(), "bcd");
    }

    TEST(StringBuilderTest, SubSequenceReturnsStringCompatibleSequence) {
        auto builder =
            jxx::NEW<StringBuilder>(
                S("abcdef"));

        auto sequence =
            builder->subSequence(
                1,
                4);

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
}