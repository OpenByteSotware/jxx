#include "lang/jxx.lang.Character.h"

#include "io/jxx.io.ObjectInputStream.h"
#include "io/jxx.io.ObjectOutputStream.h"

#include <memory>
#include <cctype>
#include <string>


#include <gsl/util>
#include <gsl/narrow>

#include "lang/jxx.lang.String.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.unicode_bridge.h"

namespace
{
    [[noreturn]] void throwIAE_(const char* message)
    {
        throw jxx::lang::IllegalArgumentException(message);
    }
}

namespace jxx::lang
{
    namespace {
        std::string normalizedUnicodeName(const jxx::Ptr<String>& value) {
            if (value == nullptr) {
                throw NullPointerException();
            }
            auto name = value->utf8();
            for (auto& ch : name) {
                if (ch == ' ' || ch == '-') ch = '_';
                else ch = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            }
            return name;
        }
    }

    Character::Subset::Subset(const jxx::Ptr<String>& name)
        : ClassBase<Subset, Object>(), name_(name) {
        if (name_ == nullptr) throw NullPointerException();
    }

    jxx::Ptr<String> Character::Subset::toString() const {
        return name_;
    }

    Character::UnicodeBlock::UnicodeBlock(
        const jxx::Ptr<String>& name, jint start, jint end)
        : ClassBase<UnicodeBlock, Subset>(name), start_(start), end_(end) {}

    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::BASIC_LATIN(
        new Character::UnicodeBlock(jxx::NEW<String>("BASIC_LATIN"), 0x0000, 0x007F));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::LATIN_1_SUPPLEMENT(
        new Character::UnicodeBlock(jxx::NEW<String>("LATIN_1_SUPPLEMENT"), 0x0080, 0x00FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GREEK(
        new Character::UnicodeBlock(jxx::NEW<String>("GREEK"), 0x0370, 0x03FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::CYRILLIC(
        new Character::UnicodeBlock(jxx::NEW<String>("CYRILLIC"), 0x0400, 0x04FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::HEBREW(
        new Character::UnicodeBlock(jxx::NEW<String>("HEBREW"), 0x0590, 0x05FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::ARABIC(
        new Character::UnicodeBlock(jxx::NEW<String>("ARABIC"), 0x0600, 0x06FF));
    const jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::GENERAL_PUNCTUATION(
        new Character::UnicodeBlock(jxx::NEW<String>("GENERAL_PUNCTUATION"), 0x2000, 0x206F));

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::of(jint codePoint) {
        if (!Character::isValidCodePoint(codePoint)) throw IllegalArgumentException();
        const jxx::Ptr<UnicodeBlock> blocks[] = {
            BASIC_LATIN, LATIN_1_SUPPLEMENT, GREEK, CYRILLIC,
            HEBREW, ARABIC, GENERAL_PUNCTUATION
        };
        for (const auto& block : blocks) {
            if (codePoint >= block->start_ && codePoint <= block->end_) return block;
        }
        return nullptr;
    }

    jxx::Ptr<Character::UnicodeBlock> Character::UnicodeBlock::forName(
        const jxx::Ptr<String>& blockName) {
        const auto requested = normalizedUnicodeName(blockName);
        const jxx::Ptr<UnicodeBlock> blocks[] = {
            BASIC_LATIN, LATIN_1_SUPPLEMENT, GREEK, CYRILLIC,
            HEBREW, ARABIC, GENERAL_PUNCTUATION
        };
        for (const auto& block : blocks) {
            if (normalizedUnicodeName(block->toString()) == requested) return block;
        }
        throw IllegalArgumentException(blockName);
    }

    Character::UnicodeScript::UnicodeScript(
        const jxx::Ptr<String>& name, jint ordinal)
        : Enum<UnicodeScript>(name, ordinal) {}

    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::COMMON(
        new Character::UnicodeScript(jxx::NEW<String>("COMMON"), 0));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::INHERITED(
        new Character::UnicodeScript(jxx::NEW<String>("INHERITED"), 1));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::LATIN(
        new Character::UnicodeScript(jxx::NEW<String>("LATIN"), 2));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::GREEK(
        new Character::UnicodeScript(jxx::NEW<String>("GREEK"), 3));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::CYRILLIC(
        new Character::UnicodeScript(jxx::NEW<String>("CYRILLIC"), 4));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HEBREW(
        new Character::UnicodeScript(jxx::NEW<String>("HEBREW"), 5));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::ARABIC(
        new Character::UnicodeScript(jxx::NEW<String>("ARABIC"), 6));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HAN(
        new Character::UnicodeScript(jxx::NEW<String>("HAN"), 7));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HIRAGANA(
        new Character::UnicodeScript(jxx::NEW<String>("HIRAGANA"), 8));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::KATAKANA(
        new Character::UnicodeScript(jxx::NEW<String>("KATAKANA"), 9));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::HANGUL(
        new Character::UnicodeScript(jxx::NEW<String>("HANGUL"), 10));
    const jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::UNKNOWN(
        new Character::UnicodeScript(jxx::NEW<String>("UNKNOWN"), 11));

    jxx::Ptr<ClassAny> Character::UnicodeScript::Class() {
        return ClassInfo<UnicodeScript, Enum<UnicodeScript>>::Class();
    }

    jxx::Ptr<JxxArray<jxx::Ptr<Character::UnicodeScript>, 1>>
    Character::UnicodeScript::values() {
        auto result = jxx::NEW<JxxArray<jxx::Ptr<UnicodeScript>, 1>>(12);
        (*result)[0]=COMMON; (*result)[1]=INHERITED; (*result)[2]=LATIN;
        (*result)[3]=GREEK; (*result)[4]=CYRILLIC; (*result)[5]=HEBREW;
        (*result)[6]=ARABIC; (*result)[7]=HAN; (*result)[8]=HIRAGANA;
        (*result)[9]=KATAKANA; (*result)[10]=HANGUL; (*result)[11]=UNKNOWN;
        return result;
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::valueOf(
        const jxx::Ptr<String>& name) {
        return Enum<UnicodeScript>::valueOf(Class(), name);
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::forName(
        const jxx::Ptr<String>& scriptName) {
        const auto requested = normalizedUnicodeName(scriptName);
        const auto all = values();
        for (std::uint32_t i=0; i<all->length; ++i) {
            const auto& script = (*all)[i];
            if (normalizedUnicodeName(script->name()) == requested) return script;
        }
        throw IllegalArgumentException(scriptName);
    }

    jxx::Ptr<Character::UnicodeScript> Character::UnicodeScript::of(jint codePoint) {
        if (!Character::isValidCodePoint(codePoint)) throw IllegalArgumentException();
        using Key = jxx::unicode_bridge::UnicodeScriptKey;
        switch (jxx::unicode_bridge::getUnicodeScript(static_cast<char32_t>(codePoint))) {
            case Key::Common: return COMMON;
            case Key::Inherited: return INHERITED;
            case Key::Latin: return LATIN;
            case Key::Greek: return GREEK;
            case Key::Cyrillic: return CYRILLIC;
            case Key::Hebrew: return HEBREW;
            case Key::Arabic: return ARABIC;
            case Key::Han: return HAN;
            case Key::Hiragana: return HIRAGANA;
            case Key::Katakana: return KATAKANA;
            case Key::Hangul: return HANGUL;
            default: return UNKNOWN;
        }
    }
    namespace {
    jxx::Ptr<ClassAny> registerCharacterPrimitive()
    {
        ClassAny::Meta metadata;
        metadata.binaryName = "char";
        metadata.typeId = std::type_index(typeid(jchar));
        metadata.isPrimitive = true;
        metadata.modifiers = 0x0001 | 0x0010 | 0x0400;
        return ClassAny::registerClass(metadata);
    }
    }

    const jxx::Ptr<ClassAny> Character::TYPE = registerCharacterPrimitive();

    jxx::Ptr<ClassAny> Character::Class()
    {
        return JxxClassInfoMarker::Class();
    }


    Character::Character(jchar value)
        : Super()
        , value_(value)
    {}

    jchar Character::charValue() const noexcept
    {
        return value_;
    }

    jint Character::compareTo(const jxx::Ptr<Character>& anotherCharacter) const
    {
        if (!anotherCharacter)
            return 1;

        if (value_ < anotherCharacter->value_)
            return -1;
        if (value_ > anotherCharacter->value_)
            return 1;
        return 0;
    }

    jbool Character::equals(const jxx::Ptr<Object>& obj) const
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

    jint Character::compare(jchar x, jchar y) noexcept
    {
        return static_cast<jint>(x) - static_cast<jint>(y);
    }

    jxx::Ptr<String> Character::toString(jchar value)
    {
        return jxx::NEW<String>(std::u16string(1, static_cast<char16_t>(value)));
    }

    jchar Character::reverseBytes(jchar value) noexcept
    {
        return static_cast<jchar>((value << 8) | (value >> 8));
    }

    jxx::Ptr<String> Character::toString() const
    {
        return String::valueOf(value_);
    }

    jxx::Ptr<Character> Character::valueOf(jchar c)
    {
        static const std::array<jxx::Ptr<Character>, 128> cache = [] {
            std::array<jxx::Ptr<Character>, 128> values{};
            for (std::size_t index = 0; index < values.size(); ++index)
            {
                values[index] = jxx::NEW<Character>(static_cast<jchar>(index));
            }
            return values;
        }();
        return c <= 127 ? cache[static_cast<std::size_t>(c)] : jxx::NEW<Character>(c);
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
            CharArray result = jxx::NEW<JxxArray<jchar, 1U>>(1U);
            (*result)[0] = static_cast<jchar>(codePoint);
            return result;
        }

        CharArray result = jxx::NEW<JxxArray<jchar, 1U>>(2U);
        (*result)[0] = highSurrogate(codePoint);
        (*result)[1] = lowSurrogate(codePoint);
        return result;
    }




    void Character::writeObject(
        const jxx::Ptr<jxx::io::ObjectOutputStream>& out)
    {
        (void)out;
    }

    void Character::readObject(
        const jxx::Ptr<jxx::io::ObjectInputStream>& in)
    {
        (void)in;
    }

    void Character::readObjectNoData()
    {
    }

}