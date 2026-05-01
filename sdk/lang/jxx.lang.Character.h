#pragma once

#include "jxx_types.h"
#include "jxx.lang.String.h"

namespace jxx::lang {

class Character final {
public:
    static constexpr jint MIN_SUPPLEMENTARY_CODE_POINT = 0x10000;
    static constexpr jint MIN_CODE_POINT = 0x000000;
    static constexpr jint MAX_CODE_POINT = 0x10FFFF;

    static jbool isHighSurrogate(jchar ch);
    static jbool isLowSurrogate(jchar ch);
    static jint toCodePoint(jchar high, jchar low);
    static jint charCount(jint codePoint);

    static jint codePointAt(jxx::Ptr<String> s, jint index);
    static jint codePointBefore(jxx::Ptr<String> s, jint index);

    static jint codePointAt(jxx::Ptr<CharArray> a, jint index);
    static jint codePointBefore(jxx::Ptr<CharArray> a, jint index);

    // Basic classification (ASCII-focused). Extend with ICU for full Unicode parity.
    static jbool isWhitespace(jint codePoint);
    static jbool isDigit(jint codePoint);
    static jbool isLetter(jint codePoint);

    static jint toLowerCase(jint codePoint);
    static jint toUpperCase(jint codePoint);
};

} // namespace jxx::lang
