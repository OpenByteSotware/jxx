#include "jxx.unicode_bridge.h"

#include <gsl/util>
#include <gsl/narrow>
#include <libunicode/ucd.h>

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

    char32_t toLowerCase(char32_t codePoint) noexcept
    {
        // ucd.h does not expose Unicode case mapping.
        if (isAsciiUpper_(codePoint))
            return static_cast<char32_t>(codePoint - U'A' + U'a');
        return codePoint;
    }

    char32_t toUpperCase(char32_t codePoint) noexcept
    {
        if (isAsciiLower_(codePoint))
            return static_cast<char32_t>(codePoint - U'a' + U'A');
        return codePoint;
    }

    char32_t toTitleCase(char32_t codePoint) noexcept
    {
        return toUpperCase(codePoint);
    }
}