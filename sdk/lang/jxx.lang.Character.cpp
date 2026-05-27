#include "jxx.lang.String.h"
#include "jxx.lang.Character.h"

namespace jxx::lang
{
    static void throwIndexOutOfBounds_()
    {
        throw std::runtime_error("Character index out of bounds");
    }

    static void throwIllegalArgument_()
    {
        throw std::runtime_error("Illegal argument");
    }

    static jbool isAsciiLower_(jchar ch)
    {
        return ch >= u'a' && ch <= u'z';
    }

    static jbool isAsciiUpper_(jchar ch)
    {
        return ch >= u'A' && ch <= u'Z';
    }

    static jbool isAsciiAlpha_(jchar ch)
    {
        return isAsciiLower_(ch) || isAsciiUpper_(ch);
    }

    static jbool isAsciiDigit_(jchar ch)
    {
        return ch >= u'0' && ch <= u'9';
    }

    static jbool isAsciiAlnum_(jchar ch)
    {
        return isAsciiAlpha_(ch) || isAsciiDigit_(ch);
    }

    static jbool isAsciiHexAlpha_(jchar ch)
    {
        return (ch >= u'a' && ch <= u'z') || (ch >= u'A' && ch <= u'Z');
    }


    Character::Character(jchar value)
        : value_(value)
    {}

    jxx::Ptr<Character> Character::valueOf(jchar value)
    {
        return jxx::NEW<Character>(value);
    }

    jchar Character::charValue() const
    {
        return value_;
    }

    jxx::Ptr<String> Character::toString() const
    {
        return Character::toString(value_);
    }

    jint Character::hashCode() const
    {
        return Character::hashCode(value_);
    }

    jbool Character::equals(const jxx::Ptr<Object> obj) const
    {
        jxx::Ptr<Character> other = jxx::CAST<Character, Object>(obj);
        return other ? (other->value_ == value_) : false;
    }

    jint Character::compareTo(const jxx::Ptr<Character> anotherCharacter) const
    {
        if (!anotherCharacter) {
            throw std::runtime_error("null Character");
        }
        return compare(value_, anotherCharacter->value_);
    }

    jint Character::hashCode(jchar value)
    {
        return static_cast<jint>(value);
    }

    jint Character::compare(jchar x, jchar y)
    {
        if (x < y) {
            return -1;
        }
        if (x > y) {
            return 1;
        }
        return 0;
    }

    jbool Character::isValidCodePoint(jint codePoint)
    {
        return codePoint >= MIN_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isBmpCodePoint(jint codePoint)
    {
        return codePoint >= MIN_CODE_POINT && codePoint <= 0xffff;
    }

    jbool Character::isSupplementaryCodePoint(jint codePoint)
    {
        return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isHighSurrogate(jchar ch)
    {
        return ch >= MIN_HIGH_SURROGATE && ch <= MAX_HIGH_SURROGATE;
    }

    jbool Character::isLowSurrogate(jchar ch)
    {
        return ch >= MIN_LOW_SURROGATE && ch <= MAX_LOW_SURROGATE;
    }

    jbool Character::isSurrogate(jchar ch)
    {
        return ch >= MIN_SURROGATE && ch <= MAX_SURROGATE;
    }

    jbool Character::isSurrogatePair(jchar high, jchar low)
    {
        return isHighSurrogate(high) && isLowSurrogate(low);
    }

    jint Character::charCount(jint codePoint)
    {
        return isSupplementaryCodePoint(codePoint) ? 2 : 1;
    }

    jint Character::toCodePoint(jchar high, jchar low)
    {
        return ((static_cast<jint>(high) - 0xd800) << 10)
            + (static_cast<jint>(low) - 0xdc00)
            + 0x10000;
    }

    jxx::Ptr<JxxArray<jchar, 1>> Character::toChars(jint codePoint)
    {
        if (!isValidCodePoint(codePoint)) {
            throwIllegalArgument_();
        }

        if (isBmpCodePoint(codePoint)) {
            auto out = jxx::NEW<CharArrayType>(1);
            (*out)[0] = static_cast<jchar>(codePoint);
            return out;
        }

        jint cpPrime = codePoint - 0x10000;
        jchar high = static_cast<jchar>((cpPrime >> 10) + 0xd800);
        jchar low = static_cast<jchar>((cpPrime & 0x3ff) + 0xdc00);

        jxx::Ptr<JxxArray<jchar, 1>> out = jxx::NEW<CharArrayType>(2);
        (*out)[0] = high;
        (*out)[1] = low;
        return out;
    }

    jint Character::toChars(jint codePoint, CharArray dst, jint dstIndex)
    {
        if (!dst) {
            throw std::runtime_error("null dst");
        }
        if (!isValidCodePoint(codePoint)) {
            throwIllegalArgument_();
        }
        if (dstIndex < 0 || dstIndex >= dst->length) {
            throwIndexOutOfBounds_();
        }

        if (isBmpCodePoint(codePoint)) {
            (*dst)[dstIndex] = static_cast<jchar>(codePoint);
            return 1;
        }

        if (dstIndex + 1 >= dst->length) {
            throwIndexOutOfBounds_();
        }

        jint cpPrime = codePoint - 0x10000;
        (*dst)[dstIndex] = static_cast<jchar>((cpPrime >> 10) + 0xd800);
        (*dst)[dstIndex + 1] = static_cast<jchar>((cpPrime & 0x3ff) + 0xdc00);
        return 2;
    }

    jint Character::codePointAt(jxx::Ptr<JxxArray<jchar, 1>> a, jint index)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        return codePointAt(a, index, a->length);
    }

    jint Character::codePointAt(jxx::Ptr<JxxArray<jchar, 1>> a, jint index, jint limit)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        if (index < 0 || limit < 0 || index >= limit || limit > a->length) {
            throwIndexOutOfBounds_();
        }

        jchar c1 = (*a)[index];
        if (isHighSurrogate(c1) && (index + 1) < limit) {
            jchar c2 = (*a)[index + 1];
            if (isLowSurrogate(c2)) {
                return toCodePoint(c1, c2);
            }
        }
        return static_cast<jint>(c1);
    }

    jint Character::codePointBefore(jxx::Ptr<JxxArray<jchar, 1>> a, jint index)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        return codePointBefore(a, index, 0);
    }

    jint Character::codePointBefore(jxx::Ptr<JxxArray<jchar, 1>> a, jint index, jint start)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        if (start < 0 || index < 0 || start >= index || index > a->length) {
            throwIndexOutOfBounds_();
        }

        jchar c2 = (*a)[index - 1];
        if (isLowSurrogate(c2) && (index - 2) >= start) {
            jchar c1 = (*a)[index - 2];
            if (isHighSurrogate(c1)) {
                return toCodePoint(c1, c2);
            }
        }

        return static_cast<jint>(c2);
    }

    jint Character::codePointCount(jxx::Ptr<JxxArray<jchar, 1>> a, jint offset, jint count)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        if (offset < 0 || count < 0 || offset + count > a->length) {
            throwIndexOutOfBounds_();
        }

        jint end = offset + count;
        jint n = 0;

        for (jint i = offset; i < end; ++i) {
            jchar c1 = (*a)[i];
            if (isHighSurrogate(c1) && (i + 1) < end && isLowSurrogate((*a)[i + 1])) {
                ++i;
            }
            ++n;
        }

        return n;
    }

    jint Character::offsetByCodePoints(
        jxx::Ptr<JxxArray<jchar, 1>> a,
        jint start,
        jint count,
        jint index,
        jint codePointOffset)
    {
        if (!a) {
            throw std::runtime_error("null array");
        }
        if (start < 0 || count < 0 || start + count > a->length || index < start || index > start + count) {
            throwIndexOutOfBounds_();
        }

        jint i = index;
        jint end = start + count;

        if (codePointOffset >= 0) {
            for (jint k = 0; k < codePointOffset; ++k) {
                if (i >= end) {
                    throwIndexOutOfBounds_();
                }
                jchar c1 = (*a)[i++];
                if (isHighSurrogate(c1) && i < end && isLowSurrogate((*a)[i])) {
                    ++i;
                }
            }
        }
        else {
            for (jint k = codePointOffset; k < 0; ++k) {
                if (i <= start) {
                    throwIndexOutOfBounds_();
                }
                jchar c2 = (*a)[--i];
                if (isLowSurrogate(c2) && i > start && isHighSurrogate((*a)[i - 1])) {
                    --i;
                }
            }
        }

        return i;
    }

    jbool Character::isDigit(jchar ch)
    {
        return isAsciiDigit_(ch);
    }

    jbool Character::isLetter(jchar ch)
    {
        return isAsciiAlpha_(ch)
            || ch == u'_'
            || (ch >= 0x00c0 && ch <= 0x02af);
    }

    jbool Character::isLetterOrDigit(jchar ch)
    {
        return isLetter(ch) || isDigit(ch);
    }

    jbool Character::isLowerCase(jchar ch)
    {
        if (isAsciiLower_(ch)) {
            return true;
        }

        // Minimal BMP-friendly fallback for Latin-1 letters
        return (ch >= 0x00df && ch <= 0x00f6) || (ch >= 0x00f8 && ch <= 0x00ff);
    }

    jbool Character::isUpperCase(jchar ch)
    {
        if (isAsciiUpper_(ch)) {
            return true;
        }

        return (ch >= 0x00c0 && ch <= 0x00d6) || (ch >= 0x00d8 && ch <= 0x00de);
    }

    jbool Character::isTitleCase(jchar ch)
    {
        // Strong core implementation; full parity requires Unicode titlecase tables.
        return isUpperCase(ch);
    }

    jbool Character::isWhitespace(jchar ch)
    {
        switch (ch) {
        case u' ':
        case u'\n':
        case u'\r':
        case u'\t':
        case u'\f':
        case 0x000b:
        case 0x001c:
        case 0x001d:
        case 0x001e:
        case 0x001f:
            return true;
        default:
            break;
        }

        // Common Unicode space separators
        return ch == 0x1680
            || ch == 0x180e
            || (ch >= 0x2000 && ch <= 0x200a)
            || ch == 0x2028
            || ch == 0x2029
            || ch == 0x205f
            || ch == 0x3000;
    }

    jbool Character::isSpaceChar(jchar ch)
    {
        return ch == u' '
            || ch == 0x00a0
            || ch == 0x1680
            || (ch >= 0x2000 && ch <= 0x200a)
            || ch == 0x2028
            || ch == 0x2029
            || ch == 0x202f
            || ch == 0x205f
            || ch == 0x3000;
    }

    jbool Character::isISOControl(jchar ch)
    {
        return (ch >= 0x0000 && ch <= 0x001f) || (ch >= 0x007f && ch <= 0x009f);
    }

    jbool Character::isJavaIdentifierStart(jchar ch)
    {
        return isLetter(ch) || ch == u'_' || ch == u'$';
    }

    jbool Character::isJavaIdentifierPart(jchar ch)
    {
        return isJavaIdentifierStart(ch) || isDigit(ch);
    }

    jbool Character::isUnicodeIdentifierStart(jchar ch)
    {
        return isLetter(ch) || ch == u'_';
    }

    jbool Character::isUnicodeIdentifierPart(jchar ch)
    {
        return isUnicodeIdentifierStart(ch) || isDigit(ch);
    }

    jbool Character::isIdentifierIgnorable(jchar ch)
    {
        return isISOControl(ch) && !isWhitespace(ch);
    }

    jint Character::digit(jchar ch, jint radix)
    {
        if (radix < MIN_RADIX || radix > MAX_RADIX) {
            return -1;
        }

        jint value = -1;

        if (ch >= u'0' && ch <= u'9') {
            value = static_cast<jint>(ch - u'0');
        }
        else if (ch >= u'a' && ch <= u'z') {
            value = static_cast<jint>(10 + (ch - u'a'));
        }
        else if (ch >= u'A' && ch <= u'Z') {
            value = static_cast<jint>(10 + (ch - u'A'));
        }

        return (value >= 0 && value < radix) ? value : -1;
    }

    jint Character::getNumericValue(jchar ch)
    {
        if (ch >= u'0' && ch <= u'9') {
            return static_cast<jint>(ch - u'0');
        }
        if (ch >= u'a' && ch <= u'z') {
            return static_cast<jint>(10 + (ch - u'a'));
        }
        if (ch >= u'A' && ch <= u'Z') {
            return static_cast<jint>(10 + (ch - u'A'));
        }
        return -1;
    }

    jchar Character::toLowerCase(jchar ch)
    {
        if (isAsciiUpper_(ch)) {
            return static_cast<jchar>(ch - u'A' + u'a');
        }

        if (ch >= 0x00c0 && ch <= 0x00d6) {
            return static_cast<jchar>(ch + 0x20);
        }
        if (ch >= 0x00d8 && ch <= 0x00de) {
            return static_cast<jchar>(ch + 0x20);
        }

        return ch;
    }

    jchar Character::toUpperCase(jchar ch)
    {
        if (isAsciiLower_(ch)) {
            return static_cast<jchar>(ch - u'a' + u'A');
        }

        if (ch >= 0x00e0 && ch <= 0x00f6) {
            return static_cast<jchar>(ch - 0x20);
        }
        if (ch >= 0x00f8 && ch <= 0x00fe) {
            return static_cast<jchar>(ch - 0x20);
        }

        return ch;
    }

    jchar Character::toTitleCase(jchar ch)
    {
        return toUpperCase(ch);
    }

    jxx::Ptr<String> Character::toString(jchar ch)
    {
        std::u16string s(1, static_cast<char16_t>(ch));
        return jxx::NEW<String>(s);
    }
}