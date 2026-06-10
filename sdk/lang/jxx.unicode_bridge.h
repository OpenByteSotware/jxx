#pragma once

#include "jxx_types.h"

namespace jxx::unicode_bridge
{
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