#pragma once

#include "jxx_types.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.Comparable.h"
#include "jxx.lang.buildin_array.h"
#include "io/jxx.io.Serializable.h"

namespace jxx::lang
{
    class String;

    class Character final
        : public Object
        , public Comparable<Character>
        , public jxx::io::Serializable
    {
    public:
        static constexpr jchar MIN_VALUE = 0x0000;
        static constexpr jchar MAX_VALUE = 0xFFFF;

        static constexpr jint MIN_RADIX = 2;
        static constexpr jint MAX_RADIX = 36;

        static constexpr jint SIZE = 16;
        static constexpr jint BYTES = 2;

        static constexpr jint MIN_CODE_POINT = 0x000000;
        static constexpr jint MAX_CODE_POINT = 0x10FFFF;
        static constexpr jint MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;

        static constexpr jchar MIN_HIGH_SURROGATE = 0xD800;
        static constexpr jchar MAX_HIGH_SURROGATE = 0xDBFF;
        static constexpr jchar MIN_LOW_SURROGATE = 0xDC00;
        static constexpr jchar MAX_LOW_SURROGATE = 0xDFFF;

        // Java Character type constants
        static constexpr jint UNASSIGNED = 0;
        static constexpr jint UPPERCASE_LETTER = 1;
        static constexpr jint LOWERCASE_LETTER = 2;
        static constexpr jint TITLECASE_LETTER = 3;
        static constexpr jint MODIFIER_LETTER = 4;
        static constexpr jint OTHER_LETTER = 5;
        static constexpr jint NON_SPACING_MARK = 6;
        static constexpr jint ENCLOSING_MARK = 7;
        static constexpr jint COMBINING_SPACING_MARK = 8;
        static constexpr jint DECIMAL_DIGIT_NUMBER = 9;
        static constexpr jint LETTER_NUMBER = 10;
        static constexpr jint OTHER_NUMBER = 11;
        static constexpr jint SPACE_SEPARATOR = 12;
        static constexpr jint LINE_SEPARATOR = 13;
        static constexpr jint PARAGRAPH_SEPARATOR = 14;
        static constexpr jint CONTROL = 15;
        static constexpr jint FORMAT = 16;
        static constexpr jint PRIVATE_USE = 18;
        static constexpr jint SURROGATE = 19;
        static constexpr jint DASH_PUNCTUATION = 20;
        static constexpr jint START_PUNCTUATION = 21;
        static constexpr jint END_PUNCTUATION = 22;
        static constexpr jint CONNECTOR_PUNCTUATION = 23;
        static constexpr jint OTHER_PUNCTUATION = 24;
        static constexpr jint MATH_SYMBOL = 25;
        static constexpr jint CURRENCY_SYMBOL = 26;
        static constexpr jint MODIFIER_SYMBOL = 27;
        static constexpr jint OTHER_SYMBOL = 28;
        static constexpr jint INITIAL_QUOTE_PUNCTUATION = 29;
        static constexpr jint FINAL_QUOTE_PUNCTUATION = 30;

    public:
        Character();
        explicit Character(jchar value);

        jchar charValue() const noexcept;

        jint compareTo(jxx::Ptr<Character> anotherCharacter) const override;
        jbool equals(jxx::Ptr<Object> obj) const override;
        jint hashCode() const override;
        jxx::Ptr<String> toString() const override;

        static jxx::Ptr<Character> valueOf(jchar c);
        static jint hashCode(jchar value) noexcept;

    public:
        static jbool isValidCodePoint(jint codePoint) noexcept;
        static jbool isBmpCodePoint(jint codePoint) noexcept;
        static jbool isSupplementaryCodePoint(jint codePoint) noexcept;

        static jbool isHighSurrogate(jchar ch) noexcept;
        static jbool isLowSurrogate(jchar ch) noexcept;
        static jbool isSurrogate(jchar ch) noexcept;
        static jbool isSurrogatePair(jchar high, jchar low) noexcept;

        static jint charCount(jint codePoint) noexcept;

        static jint toCodePoint(jchar high, jchar low) noexcept;
        static jchar highSurrogate(jint codePoint) noexcept;
        static jchar lowSurrogate(jint codePoint) noexcept;

        static jbool isLowerCase(jint codePoint) noexcept;
        static jbool isUpperCase(jint codePoint) noexcept;
        static jbool isTitleCase(jint codePoint) noexcept;
        static jbool isDigit(jint codePoint) noexcept;
        static jbool isLetter(jint codePoint) noexcept;
        static jbool isLetterOrDigit(jint codePoint) noexcept;
        static jbool isWhitespace(jint codePoint) noexcept;
        static jbool isSpaceChar(jint codePoint) noexcept;
        static jbool isISOControl(jint codePoint) noexcept;
        static jbool isDefined(jint codePoint) noexcept;
        static jbool isMirrored(jint codePoint) noexcept;

        static jint getType(jint codePoint) noexcept;
        static jint getNumericValue(jint codePoint) noexcept;
        static jint digit(jint codePoint, jint radix) noexcept;

        static jint toLowerCase(jint codePoint) noexcept;
        static jint toUpperCase(jint codePoint) noexcept;
        static jint toTitleCase(jint codePoint) noexcept;

        static CharArray toChars(jint codePoint);

        virtual void writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out);
        virtual void readObject(jxx::Ptr<jxx::io::ObjectInputStream> in);
        virtual void readObjectNoData();

    private:
        jchar value_;
    };
}