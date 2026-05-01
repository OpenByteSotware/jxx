#include "jxx.lang.Character.h"

#include <stdexcept>

namespace jxx::lang {

jbool Character::isHighSurrogate(jchar ch) { return ch >= 0xD800 && ch <= 0xDBFF; }
jbool Character::isLowSurrogate(jchar ch)  { return ch >= 0xDC00 && ch <= 0xDFFF; }

jint Character::toCodePoint(jchar high, jchar low) {
    return (((jint)high - 0xD800) << 10) + ((jint)low - 0xDC00) + 0x10000;
}

jint Character::charCount(jint codePoint) {
    return (codePoint >= MIN_SUPPLEMENTARY_CODE_POINT) ? 2 : 1;
}

jint Character::codePointAt(jxx::Ptr<String> s, jint index) {
    if (!s) throw std::invalid_argument("NullPointerException");
    return s->codePointAt(index);
}

jint Character::codePointBefore(jxx::Ptr<String> s, jint index) {
    if (!s) throw std::invalid_argument("NullPointerException");
    return s->codePointBefore(index);
}

jint Character::codePointAt(jxx::Ptr<CharArray> a, jint index) {
    if (!a) throw std::invalid_argument("NullPointerException");
    if (index < 0 || (std::uint32_t)index >= a->length) throw std::out_of_range("StringIndexOutOfBoundsException");

    jchar c1 = (*a)[index];
    if (isHighSurrogate(c1) && (std::uint32_t)(index + 1) < a->length) {
        jchar c2 = (*a)[index + 1];
        if (isLowSurrogate(c2)) return toCodePoint(c1, c2);
    }
    return (jint)c1;
}

jint Character::codePointBefore(jxx::Ptr<CharArray> a, jint index) {
    if (!a) throw std::invalid_argument("NullPointerException");
    if (index <= 0 || (std::uint32_t)index > a->length) throw std::out_of_range("StringIndexOutOfBoundsException");

    jchar c2 = (*a)[index - 1];
    if (isLowSurrogate(c2) && index - 2 >= 0) {
        jchar c1 = (*a)[index - 2];
        if (isHighSurrogate(c1)) return toCodePoint(c1, c2);
    }
    return (jint)c2;
}

jbool Character::isWhitespace(jint cp) {
    return cp == 0x20 || (cp >= 0x09 && cp <= 0x0D);
}

jbool Character::isDigit(jint cp) { return (cp >= '0' && cp <= '9'); }

jbool Character::isLetter(jint cp) { return (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z'); }

jint Character::toLowerCase(jint cp) {
    if (cp >= 'A' && cp <= 'Z') return cp - 'A' + 'a';
    return cp;
}

jint Character::toUpperCase(jint cp) {
    if (cp >= 'a' && cp <= 'z') return cp - 'a' + 'A';
    return cp;
}

} // namespace jxx::lang
