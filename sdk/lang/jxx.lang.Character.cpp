#include "jxx.lang.Character.h"

#include <memory>
#include <stdexcept>

#include <gsl/util>
#include <gsl/narrow>

#include "jxx.lang.String.h"
#include "jxx.unicode_bridge.h"

namespace
{
    [[noreturn]] void throwIAE_(const char* message)
    {
        throw std::invalid_argument(message);
    }
}

namespace jxx::lang
{
    Character::Character()
        : value_(0)
    {}

    Character::Character(jchar value)
        : value_(value)
    {}

    jchar Character::charValue() const noexcept
    {
        return value_;
    }

    jint Character::compareTo(jxx::Ptr<Character> anotherCharacter) const
    {
        if (!anotherCharacter)
            return 1;

        if (value_ < anotherCharacter->value_)
            return -1;
        if (value_ > anotherCharacter->value_)
            return 1;
        return 0;
    }

    jbool Character::equals(jxx::Ptr<Object> obj) const
    {
        auto other = std::dynamic_pointer_cast<Character>(obj);
        return other && other->value_ == value_;
    }

    jint Character::hashCode() const
    {
        return hashCode(value_);
    }

    jint Character::hashCode(jchar value) noexcept
    {
        return static_cast<jint>(value);
    }

    jxx::Ptr<String> Character::toString() const
    {
        return String::valueOf(value_);
    }

    jxx::Ptr<Character> Character::valueOf(jchar c)
    {
        return std::make_shared<Character>(c);
    }

    jbool Character::isValidCodePoint(jint codePoint) noexcept
    {
        return codePoint >= MIN_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isBmpCodePoint(jint codePoint) noexcept
    {
        return codePoint >= 0x0000 && codePoint <= 0xFFFF;
    }

    jbool Character::isSupplementaryCodePoint(jint codePoint) noexcept
    {
        return codePoint >= MIN_SUPPLEMENTARY_CODE_POINT && codePoint <= MAX_CODE_POINT;
    }

    jbool Character::isHighSurrogate(jchar ch) noexcept
    {
        return ch >= MIN_HIGH_SURROGATE && ch <= MAX_HIGH_SURROGATE;
    }

    jbool Character::isLowSurrogate(jchar ch) noexcept
    {
        return ch >= MIN_LOW_SURROGATE && ch <= MAX_LOW_SURROGATE;
    }

    jbool Character::isSurrogate(jchar ch) noexcept
    {
        return isHighSurrogate(ch) || isLowSurrogate(ch);
    }

    jbool Character::isSurrogatePair(jchar high, jchar low) noexcept
    {
        return isHighSurrogate(high) && isLowSurrogate(low);
    }

    jint Character::charCount(jint codePoint) noexcept
    {
        return isSupplementaryCodePoint(codePoint) ? 2 : 1;
    }

    jint Character::toCodePoint(jchar high, jchar low) noexcept
    {
        return ((static_cast<jint>(high) - 0xD800) << 10)
            + (static_cast<jint>(low) - 0xDC00)
            + 0x10000;
    }

    jchar Character::highSurrogate(jint codePoint) noexcept
    {
        return static_cast<jchar>(((codePoint - 0x10000) >> 10) + 0xD800);
    }

    jchar Character::lowSurrogate(jint codePoint) noexcept
    {
        return static_cast<jchar>(((codePoint - 0x10000) & 0x3FF) + 0xDC00);
    }

    jbool Character::isLowerCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLowerCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isUpperCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isUpperCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isTitleCase(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isTitleCase(static_cast<char32_t>(codePoint));
    }

    jbool Character::isDigit(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isDigit(static_cast<char32_t>(codePoint));
    }

    jbool Character::isLetter(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLetter(static_cast<char32_t>(codePoint));
    }

    jbool Character::isLetterOrDigit(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isLetterOrDigit(static_cast<char32_t>(codePoint));
    }

    jbool Character::isWhitespace(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isWhitespace(static_cast<char32_t>(codePoint));
    }

    jbool Character::isSpaceChar(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isSpaceChar(static_cast<char32_t>(codePoint));
    }

    jbool Character::isISOControl(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isISOControl(static_cast<char32_t>(codePoint));
    }

    jbool Character::isDefined(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isDefined(static_cast<char32_t>(codePoint));
    }

    jbool Character::isMirrored(jint codePoint) noexcept
    {
        return isValidCodePoint(codePoint)
            && jxx::unicode_bridge::isMirrored(static_cast<char32_t>(codePoint));
    }

    jint Character::getType(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return UNASSIGNED;

        return jxx::unicode_bridge::getType(static_cast<char32_t>(codePoint));
    }

    jint Character::getNumericValue(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return -1;

        return jxx::unicode_bridge::getNumericValue(static_cast<char32_t>(codePoint));
    }

    jint Character::digit(jint codePoint, jint radix) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return -1;

        if (radix < MIN_RADIX || radix > MAX_RADIX)
            return -1;

        return jxx::unicode_bridge::digit(static_cast<char32_t>(codePoint), radix);
    }

    jint Character::toLowerCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toLowerCase(static_cast<char32_t>(codePoint)));
    }

    jint Character::toUpperCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toUpperCase(static_cast<char32_t>(codePoint)));
    }

    jint Character::toTitleCase(jint codePoint) noexcept
    {
        if (!isValidCodePoint(codePoint))
            return codePoint;

        return gsl::narrow<jint>(
            jxx::unicode_bridge::toTitleCase(static_cast<char32_t>(codePoint)));
    }

    CharArray Character::toChars(jint codePoint)
    {
        if (!isValidCodePoint(codePoint))
            throwIAE_("invalid Unicode code point");

        if (isBmpCodePoint(codePoint))
        {
            CharArray result = std::make_shared<JxxArray<jchar, 1U>>(1U);
            (*result)[0] = static_cast<jchar>(codePoint);
            return result;
        }

        CharArray result = std::make_shared<JxxArray<jchar, 1U>>(2U);
        (*result)[0] = highSurrogate(codePoint);
        (*result)[1] = lowSurrogate(codePoint);
        return result;
    }

    void Character::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out)
    {


    }

    void Character::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {

    }
    void Character::readObjectNoData() {

    }
}