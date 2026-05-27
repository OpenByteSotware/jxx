#pragma once
#include <string>
#include <stdexcept>
#include <cstddef>
#include "jxx_types.h"
#include "jxx.lang.buildin_array.h"
#include "jxx.lang.Object.h"

namespace jxx::lang {
    class Character final : public Object {
    public:
        static constexpr jchar MIN_VALUE = static_cast<jchar>(0x0000);
        static constexpr jchar MAX_VALUE = static_cast<jchar>(0xffff);

        static constexpr jint MIN_RADIX = 2;
        static constexpr jint MAX_RADIX = 36;

        static constexpr jint MIN_CODE_POINT = 0x000000;
        static constexpr jint MAX_CODE_POINT = 0x10ffff;
        static constexpr jint MIN_SUPPLEMENTARY_CODE_POINT = 0x010000;

        static constexpr jchar MIN_HIGH_SURROGATE = static_cast<jchar>(0xd800);
        static constexpr jchar MAX_HIGH_SURROGATE = static_cast<jchar>(0xdbff);
        static constexpr jchar MIN_LOW_SURROGATE = static_cast<jchar>(0xdc00);
        static constexpr jchar MAX_LOW_SURROGATE = static_cast<jchar>(0xdfff);

        static constexpr jchar MIN_SURROGATE = MIN_HIGH_SURROGATE;
        static constexpr jchar MAX_SURROGATE = MAX_LOW_SURROGATE;

        static constexpr jint SIZE = 16;
        static constexpr jint BYTES = 2;

    public:
        explicit Character(jchar value);

    public:
        static jxx::Ptr<Character> valueOf(jchar value);

    public:
        jchar charValue() const;

        jxx::Ptr<String> toString() const override;
        jint hashCode() const override;
        jbool equals(const jxx::Ptr<Object> obj) const override;

        jint compareTo(const jxx::Ptr<Character> anotherCharacter) const;

    public:
        static jint hashCode(jchar value);
        static jint compare(jchar x, jchar y);

    public:
        static jbool isValidCodePoint(jint codePoint);
        static jbool isBmpCodePoint(jint codePoint);
        static jbool isSupplementaryCodePoint(jint codePoint);

        static jbool isHighSurrogate(jchar ch);
        static jbool isLowSurrogate(jchar ch);
        static jbool isSurrogate(jchar ch);
        static jbool isSurrogatePair(jchar high, jchar low);

        static jint charCount(jint codePoint);
        static jint toCodePoint(jchar high, jchar low);

        static CharArray toChars(jint codePoint);
        static jint toChars(jint codePoint, CharArray dst, jint dstIndex);

    public:
        static jint codePointAt(CharArray a, jint index);
        static jint codePointAt(CharArray a, jint index, jint limit);

        static jint codePointBefore(CharArray a, jint index);
        static jint codePointBefore(CharArray a, jint index, jint start);
        static jint codePointCount(CharArray a, jint offset, jint count);
        static jint offsetByCodePoints(CharArray a, jint start, jint count, jint index, jint codePointOffset);

    public:
        static jbool isDigit(jchar ch);
        static jbool isLetter(jchar ch);
        static jbool isLetterOrDigit(jchar ch);
        static jbool isLowerCase(jchar ch);
        static jbool isUpperCase(jchar ch);
        static jbool isTitleCase(jchar ch);
        static jbool isWhitespace(jchar ch);
        static jbool isSpaceChar(jchar ch);
        static jbool isISOControl(jchar ch);

        static jbool isJavaIdentifierStart(jchar ch);
        static jbool isJavaIdentifierPart(jchar ch);
        static jbool isUnicodeIdentifierStart(jchar ch);
        static jbool isUnicodeIdentifierPart(jchar ch);
        static jbool isIdentifierIgnorable(jchar ch);

    public:
        static jint digit(jchar ch, jint radix);
        static jint getNumericValue(jchar ch);

    public:
        static jchar toLowerCase(jchar ch);
        static jchar toUpperCase(jchar ch);
        static jchar toTitleCase(jchar ch);

    public:
        static jxx::Ptr<String> toString(jchar ch);

    private:
        jchar value_;
    };
}