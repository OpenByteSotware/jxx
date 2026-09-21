#pragma once

#include "jxx_types.h"

namespace jxx::unicode_bridge
{
    enum class UnicodeScriptKey : unsigned char
    {
        Common,
        Latin,
        Greek,
        Cyrillic,
        Armenian,
        Hebrew,
        Arabic,
        Syriac,
        Thaana,
        Devanagari,
        Bengali,
        Gurmukhi,
        Gujarati,
        Oriya,
        Tamil,
        Telugu,
        Kannada,
        Malayalam,
        Sinhala,
        Thai,
        Lao,
        Tibetan,
        Myanmar,
        Georgian,
        Hangul,
        Ethiopic,
        Cherokee,
        CanadianAboriginal,
        Ogham,
        Runic,
        Khmer,
        Mongolian,
        Hiragana,
        Katakana,
        Bopomofo,
        Han,
        Yi,
        OldItalic,
        Gothic,
        Deseret,
        Inherited,
        Tagalog,
        Hanunoo,
        Buhid,
        Tagbanwa,
        Limbu,
        TaiLe,
        LinearB,
        Ugaritic,
        Shavian,
        Osmanya,
        Cypriot,
        Braille,
        Buginese,
        Coptic,
        NewTaiLue,
        Glagolitic,
        Tifinagh,
        SylotiNagri,
        OldPersian,
        Kharoshthi,
        Balinese,
        Cuneiform,
        Phoenician,
        PhagsPa,
        Nko,
        Sundanese,
        Lepcha,
        OlChiki,
        Vai,
        Saurashtra,
        KayahLi,
        Rejang,
        Lycian,
        Carian,
        Lydian,
        Cham,
        TaiTham,
        TaiViet,
        Avestan,
        EgyptianHieroglyphs,
        Samaritan,
        Mandaic,
        Lisu,
        Bamum,
        Javanese,
        MeeteiMayek,
        ImperialAramaic,
        OldSouthArabian,
        InscriptionalParthian,
        InscriptionalPahlavi,
        OldTurkic,
        Brahmi,
        Kaithi,
        MeroiticHieroglyphs,
        MeroiticCursive,
        SoraSompeng,
        Chakma,
        Sharada,
        Takri,
        Miao,
        Unknown,
    };

    UnicodeScriptKey getUnicodeScript(char32_t codePoint) noexcept;

    bool isLowerCase(char32_t codePoint) noexcept;
    bool isUpperCase(char32_t codePoint) noexcept;
    bool isTitleCase(char32_t codePoint) noexcept;
    bool isDigit(char32_t codePoint) noexcept;
    bool isLetter(char32_t codePoint) noexcept;
    bool isLetterOrDigit(char32_t codePoint) noexcept;
    bool isWhitespace(char32_t codePoint) noexcept;
    bool isSpaceChar(char32_t codePoint) noexcept;
    bool isISOControl(char32_t codePoint) noexcept;

    bool isDefined(char32_t codePoint) noexcept;
    bool isMirrored(char32_t codePoint) noexcept;

    jxx::lang::jint getType(char32_t codePoint) noexcept;
    jxx::lang::jint getNumericValue(char32_t codePoint) noexcept;
    jxx::lang::jint digit(char32_t codePoint, jxx::lang::jint radix) noexcept;

    char32_t toLowerCase(char32_t codePoint) noexcept;
    char32_t toUpperCase(char32_t codePoint) noexcept;
    char32_t toTitleCase(char32_t codePoint) noexcept;
}