#include "jxx.unicode_bridge.h"

#include <gsl/util>
#include <gsl/narrow>
#include <libunicode/ucd.h>
#include <libunicode/case_mapping.h>
#include <libunicode/codepoint_properties.h>

namespace
{
    inline bool isAsciiLower_(char32_t cp) noexcept
    {
        return cp >= U'a' && cp <= U'z';
    }

    inline bool isAsciiUpper_(char32_t cp) noexcept
    {
        return cp >= U'A' && cp <= U'Z';
    }

    inline bool isAsciiDigit_(char32_t cp) noexcept
    {
        return cp >= U'0' && cp <= U'9';
    }

    inline jxx::lang::jint asciiNumericValue_(char32_t cp) noexcept
    {
        if (isAsciiDigit_(cp))
            return gsl::narrow<jxx::lang::jint>(cp - U'0');

        if (cp >= U'a' && cp <= U'z')
            return gsl::narrow<jxx::lang::jint>(cp - U'a' + 10);

        if (cp >= U'A' && cp <= U'Z')
            return gsl::narrow<jxx::lang::jint>(cp - U'A' + 10);

        return -1;
    }
}

namespace jxx::unicode_bridge
{
    bool isLowerCase(char32_t codePoint) noexcept
    {
        return unicode::general_category::is_lowercase_letter(codePoint);
    }

    bool isUpperCase(char32_t codePoint) noexcept
    {
        return unicode::general_category::is_uppercase_letter(codePoint);
    }

    bool isTitleCase(char32_t codePoint) noexcept
    {
        return unicode::general_category::is_titlecase_letter(codePoint);
    }

    bool isDigit(char32_t codePoint) noexcept
    {
        return unicode::general_category::is_decimal_number(codePoint);
    }

    bool isLetter(char32_t codePoint) noexcept
    {
        using namespace unicode::general_category;

        return is_uppercase_letter(codePoint)
            || is_lowercase_letter(codePoint)
            || is_titlecase_letter(codePoint)
            || is_modifier_letter(codePoint)
            || is_other_letter(codePoint);
    }

    bool isLetterOrDigit(char32_t codePoint) noexcept
    {
        return isLetter(codePoint) || isDigit(codePoint);
    }

    bool isWhitespace(char32_t codePoint) noexcept
    {
        using namespace unicode::general_category;

        // Approximation of Java Character.isWhitespace(int):
        // separator classes + ASCII control whitespace characters.
        if (is_space_separator(codePoint)
            || is_line_separator(codePoint)
            || is_paragraph_separator(codePoint))
        {
            return true;
        }

        switch (codePoint)
        {
            case 0x0009: // HT
            case 0x000A: // LF
            case 0x000B: // VT
            case 0x000C: // FF
            case 0x000D: // CR
            case 0x001C: // FS
            case 0x001D: // GS
            case 0x001E: // RS
            case 0x001F: // US
                return true;
            default:
                return false;
        }
    }

    bool isSpaceChar(char32_t codePoint) noexcept
    {
        using namespace unicode::general_category;

        return is_space_separator(codePoint)
            || is_line_separator(codePoint)
            || is_paragraph_separator(codePoint);
    }

    bool isISOControl(char32_t codePoint) noexcept
    {
        return (codePoint <= 0x001F)
            || (codePoint >= 0x007F && codePoint <= 0x009F);
    }

    bool isDefined(char32_t codePoint) noexcept
    {
        return !unicode::general_category::is_unassigned(codePoint);
    }

    bool isMirrored(char32_t codePoint) noexcept
    {
        return unicode::is_mirrored(codePoint);
    }

    jxx::lang::jint getType(char32_t codePoint) noexcept
    {
        switch (unicode::general_category::get(codePoint))
        {
            case unicode::General_Category::Unassigned:            return 0;
            case unicode::General_Category::Uppercase_Letter:      return 1;
            case unicode::General_Category::Lowercase_Letter:      return 2;
            case unicode::General_Category::Titlecase_Letter:      return 3;
            case unicode::General_Category::Modifier_Letter:       return 4;
            case unicode::General_Category::Other_Letter:          return 5;
            case unicode::General_Category::Nonspacing_Mark:       return 6;
            case unicode::General_Category::Enclosing_Mark:        return 7;
            case unicode::General_Category::Spacing_Mark:          return 8;
            case unicode::General_Category::Decimal_Number:        return 9;
            case unicode::General_Category::Letter_Number:         return 10;
            case unicode::General_Category::Other_Number:          return 11;
            case unicode::General_Category::Space_Separator:       return 12;
            case unicode::General_Category::Line_Separator:        return 13;
            case unicode::General_Category::Paragraph_Separator:   return 14;
            case unicode::General_Category::Control:               return 15;
            case unicode::General_Category::Format:                return 16;
            case unicode::General_Category::Private_Use:           return 18;
            case unicode::General_Category::Surrogate:             return 19;
            case unicode::General_Category::Dash_Punctuation:      return 20;
            case unicode::General_Category::Open_Punctuation:      return 21;
            case unicode::General_Category::Close_Punctuation:     return 22;
            case unicode::General_Category::Connector_Punctuation: return 23;
            case unicode::General_Category::Other_Punctuation:     return 24;
            case unicode::General_Category::Math_Symbol:           return 25;
            case unicode::General_Category::Currency_Symbol:       return 26;
            case unicode::General_Category::Modifier_Symbol:       return 27;
            case unicode::General_Category::Other_Symbol:          return 28;
            case unicode::General_Category::Initial_Punctuation:   return 29;
            case unicode::General_Category::Final_Punctuation:     return 30;
        }

        return 0;
    }

    jxx::lang::jint getNumericValue(char32_t codePoint) noexcept
    {
        // ucd.h does not expose a Unicode numeric-value accessor.
        // ASCII/Latin-letter fallback for now.
        return asciiNumericValue_(codePoint);
    }

    jxx::lang::jint digit(char32_t codePoint, jxx::lang::jint radix) noexcept
    {
        if (radix < 2 || radix > 36)
            return -1;

        const jxx::lang::jint value = getNumericValue(codePoint);
        return (value >= 0 && value < radix) ? value : -1;
    }

    UnicodeScriptKey getUnicodeScript(char32_t codePoint) noexcept
    {
        using unicode::Script;
        switch (unicode::codepoint_properties::get(codePoint).script)
        {
            case Script::Common: return UnicodeScriptKey::Common;
            case Script::Latin: return UnicodeScriptKey::Latin;
            case Script::Greek: return UnicodeScriptKey::Greek;
            case Script::Cyrillic: return UnicodeScriptKey::Cyrillic;
            case Script::Armenian: return UnicodeScriptKey::Armenian;
            case Script::Hebrew: return UnicodeScriptKey::Hebrew;
            case Script::Arabic: return UnicodeScriptKey::Arabic;
            case Script::Syriac: return UnicodeScriptKey::Syriac;
            case Script::Thaana: return UnicodeScriptKey::Thaana;
            case Script::Devanagari: return UnicodeScriptKey::Devanagari;
            case Script::Bengali: return UnicodeScriptKey::Bengali;
            case Script::Gurmukhi: return UnicodeScriptKey::Gurmukhi;
            case Script::Gujarati: return UnicodeScriptKey::Gujarati;
            case Script::Oriya: return UnicodeScriptKey::Oriya;
            case Script::Tamil: return UnicodeScriptKey::Tamil;
            case Script::Telugu: return UnicodeScriptKey::Telugu;
            case Script::Kannada: return UnicodeScriptKey::Kannada;
            case Script::Malayalam: return UnicodeScriptKey::Malayalam;
            case Script::Sinhala: return UnicodeScriptKey::Sinhala;
            case Script::Thai: return UnicodeScriptKey::Thai;
            case Script::Lao: return UnicodeScriptKey::Lao;
            case Script::Tibetan: return UnicodeScriptKey::Tibetan;
            case Script::Myanmar: return UnicodeScriptKey::Myanmar;
            case Script::Georgian: return UnicodeScriptKey::Georgian;
            case Script::Hangul: return UnicodeScriptKey::Hangul;
            case Script::Ethiopic: return UnicodeScriptKey::Ethiopic;
            case Script::Cherokee: return UnicodeScriptKey::Cherokee;
            case Script::Canadian_Aboriginal: return UnicodeScriptKey::CanadianAboriginal;
            case Script::Ogham: return UnicodeScriptKey::Ogham;
            case Script::Runic: return UnicodeScriptKey::Runic;
            case Script::Khmer: return UnicodeScriptKey::Khmer;
            case Script::Mongolian: return UnicodeScriptKey::Mongolian;
            case Script::Hiragana: return UnicodeScriptKey::Hiragana;
            case Script::Katakana: return UnicodeScriptKey::Katakana;
            case Script::Bopomofo: return UnicodeScriptKey::Bopomofo;
            case Script::Han: return UnicodeScriptKey::Han;
            case Script::Yi: return UnicodeScriptKey::Yi;
            case Script::Old_Italic: return UnicodeScriptKey::OldItalic;
            case Script::Gothic: return UnicodeScriptKey::Gothic;
            case Script::Deseret: return UnicodeScriptKey::Deseret;
            case Script::Inherited: return UnicodeScriptKey::Inherited;
            case Script::Tagalog: return UnicodeScriptKey::Tagalog;
            case Script::Hanunoo: return UnicodeScriptKey::Hanunoo;
            case Script::Buhid: return UnicodeScriptKey::Buhid;
            case Script::Tagbanwa: return UnicodeScriptKey::Tagbanwa;
            case Script::Limbu: return UnicodeScriptKey::Limbu;
            case Script::Tai_Le: return UnicodeScriptKey::TaiLe;
            case Script::Linear_B: return UnicodeScriptKey::LinearB;
            case Script::Ugaritic: return UnicodeScriptKey::Ugaritic;
            case Script::Shavian: return UnicodeScriptKey::Shavian;
            case Script::Osmanya: return UnicodeScriptKey::Osmanya;
            case Script::Cypriot: return UnicodeScriptKey::Cypriot;
            case Script::Braille: return UnicodeScriptKey::Braille;
            case Script::Buginese: return UnicodeScriptKey::Buginese;
            case Script::Coptic: return UnicodeScriptKey::Coptic;
            case Script::New_Tai_Lue: return UnicodeScriptKey::NewTaiLue;
            case Script::Glagolitic: return UnicodeScriptKey::Glagolitic;
            case Script::Tifinagh: return UnicodeScriptKey::Tifinagh;
            case Script::Syloti_Nagri: return UnicodeScriptKey::SylotiNagri;
            case Script::Old_Persian: return UnicodeScriptKey::OldPersian;
            case Script::Kharoshthi: return UnicodeScriptKey::Kharoshthi;
            case Script::Balinese: return UnicodeScriptKey::Balinese;
            case Script::Cuneiform: return UnicodeScriptKey::Cuneiform;
            case Script::Phoenician: return UnicodeScriptKey::Phoenician;
            case Script::Phags_Pa: return UnicodeScriptKey::PhagsPa;
            case Script::Nko: return UnicodeScriptKey::Nko;
            case Script::Sundanese: return UnicodeScriptKey::Sundanese;
            case Script::Lepcha: return UnicodeScriptKey::Lepcha;
            case Script::Ol_Chiki: return UnicodeScriptKey::OlChiki;
            case Script::Vai: return UnicodeScriptKey::Vai;
            case Script::Saurashtra: return UnicodeScriptKey::Saurashtra;
            case Script::Kayah_Li: return UnicodeScriptKey::KayahLi;
            case Script::Rejang: return UnicodeScriptKey::Rejang;
            case Script::Lycian: return UnicodeScriptKey::Lycian;
            case Script::Carian: return UnicodeScriptKey::Carian;
            case Script::Lydian: return UnicodeScriptKey::Lydian;
            case Script::Cham: return UnicodeScriptKey::Cham;
            case Script::Tai_Tham: return UnicodeScriptKey::TaiTham;
            case Script::Tai_Viet: return UnicodeScriptKey::TaiViet;
            case Script::Avestan: return UnicodeScriptKey::Avestan;
            case Script::Egyptian_Hieroglyphs: return UnicodeScriptKey::EgyptianHieroglyphs;
            case Script::Samaritan: return UnicodeScriptKey::Samaritan;
            case Script::Mandaic: return UnicodeScriptKey::Mandaic;
            case Script::Lisu: return UnicodeScriptKey::Lisu;
            case Script::Bamum: return UnicodeScriptKey::Bamum;
            case Script::Javanese: return UnicodeScriptKey::Javanese;
            case Script::Meetei_Mayek: return UnicodeScriptKey::MeeteiMayek;
            case Script::Imperial_Aramaic: return UnicodeScriptKey::ImperialAramaic;
            case Script::Old_South_Arabian: return UnicodeScriptKey::OldSouthArabian;
            case Script::Inscriptional_Parthian: return UnicodeScriptKey::InscriptionalParthian;
            case Script::Inscriptional_Pahlavi: return UnicodeScriptKey::InscriptionalPahlavi;
            case Script::Old_Turkic: return UnicodeScriptKey::OldTurkic;
            case Script::Brahmi: return UnicodeScriptKey::Brahmi;
            case Script::Kaithi: return UnicodeScriptKey::Kaithi;
            case Script::Meroitic_Hieroglyphs: return UnicodeScriptKey::MeroiticHieroglyphs;
            case Script::Meroitic_Cursive: return UnicodeScriptKey::MeroiticCursive;
            case Script::Sora_Sompeng: return UnicodeScriptKey::SoraSompeng;
            case Script::Chakma: return UnicodeScriptKey::Chakma;
            case Script::Sharada: return UnicodeScriptKey::Sharada;
            case Script::Takri: return UnicodeScriptKey::Takri;
            case Script::Miao: return UnicodeScriptKey::Miao;
            default: return UnicodeScriptKey::Unknown;
        }
    }

    char32_t toLowerCase(char32_t codePoint) noexcept
    {
        return unicode::simple_lowercase(codePoint);
    }

    char32_t toUpperCase(char32_t codePoint) noexcept
    {
        return unicode::simple_uppercase(codePoint);
    }

    char32_t toTitleCase(char32_t codePoint) noexcept
    {
        return unicode::simple_titlecase(codePoint);
    }
}