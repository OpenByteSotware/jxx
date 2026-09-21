#include "lang/jxx.lang.Integer.h"
#include "lang/jxx.lang.Long.h"

#include <algorithm>
#include <cstdint>
#include <limits>
#include <string>

#include "lang/jxx.lang.ArithmeticException.h"
#include "lang/jxx.lang.NumberFormatException.h"
#include "lang/jxx.lang.NullPointerException.h"
#include "lang/jxx.lang.String.h"

namespace jxx::lang {
namespace {
constexpr char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

template <typename U>
::jxx::Ptr<String> unsignedText(U value, jint radix) {
    if (radix < 2 || radix > 36) radix = 10;
    std::string text;
    do {
        text.push_back(digits[value % static_cast<U>(radix)]);
        value /= static_cast<U>(radix);
    } while (value != 0);
    std::reverse(text.begin(), text.end());
    return ::jxx::NEW<String>(text);
}

template <typename U>
U parseUnsigned(const ::jxx::Ptr<String>& value, jint radix) {
    if (value == nullptr) throw NullPointerException();
    if (radix < 2 || radix > 36) throw NumberFormatException("radix out of range");
    const auto text = value->utf8();
    if (text.empty() || text.front() == '-') throw NumberFormatException();
    U result = 0;
    for (const auto ch : text) {
        jint digit = -1;
        if (ch >= '0' && ch <= '9') digit = ch - '0';
        else if (ch >= 'a' && ch <= 'z') digit = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'Z') digit = ch - 'A' + 10;
        if (digit < 0 || digit >= radix) throw NumberFormatException();
        const auto unsignedDigit = static_cast<U>(digit);
        if (result > (std::numeric_limits<U>::max() - unsignedDigit) / static_cast<U>(radix)) {
            throw NumberFormatException();
        }
        result = static_cast<U>(result * static_cast<U>(radix) + unsignedDigit);
    }
    return result;
}

template <typename U>
jint leadingZeros(U value) noexcept {
    constexpr jint width = static_cast<jint>(sizeof(U) * 8U);
    if (value == 0) return width;
    jint count = 0;
    for (U bit = static_cast<U>(U{1} << (width - 1)); (value & bit) == 0; bit >>= 1) ++count;
    return count;
}

template <typename U>
jint trailingZeros(U value) noexcept {
    constexpr jint width = static_cast<jint>(sizeof(U) * 8U);
    if (value == 0) return width;
    jint count = 0;
    while ((value & U{1}) == 0) { value >>= 1; ++count; }
    return count;
}

template <typename U>
jint popcount(U value) noexcept {
    jint count = 0;
    while (value != 0) { value &= static_cast<U>(value - 1); ++count; }
    return count;
}

template <typename U>
U reverseBits(U value) noexcept {
    U result = 0;
    for (std::size_t i = 0; i < sizeof(U) * 8U; ++i) {
        result = static_cast<U>((result << 1) | (value & U{1}));
        value >>= 1;
    }
    return result;
}
} // namespace

::jxx::Ptr<String> Integer::toString(jint value, jint radix) {
    if (radix < 2 || radix > 36) radix = 10;
    if (value >= 0) return unsignedText(static_cast<std::uint32_t>(value), radix);
    const auto magnitude = static_cast<std::uint32_t>(-(static_cast<std::int64_t>(value)));
    return ::jxx::NEW<String>("-" + unsignedText(magnitude, radix)->utf8());
}
::jxx::Ptr<String> Integer::toUnsignedString(jint value) { return toUnsignedString(value, 10); }
::jxx::Ptr<String> Integer::toUnsignedString(jint value, jint radix) { return unsignedText(static_cast<std::uint32_t>(value), radix); }
::jxx::Ptr<String> Integer::toHexString(jint value) { return toUnsignedString(value, 16); }
::jxx::Ptr<String> Integer::toOctalString(jint value) { return toUnsignedString(value, 8); }
::jxx::Ptr<String> Integer::toBinaryString(jint value) { return toUnsignedString(value, 2); }
jint Integer::parseUnsignedInt(const ::jxx::Ptr<String>& value) { return parseUnsignedInt(value, 10); }
jint Integer::parseUnsignedInt(const ::jxx::Ptr<String>& value, jint radix) { return static_cast<jint>(parseUnsigned<std::uint32_t>(value, radix)); }
jint Integer::compareUnsigned(jint a, jint b) noexcept { const auto x=static_cast<std::uint32_t>(a), y=static_cast<std::uint32_t>(b); return x<y?-1:x>y?1:0; }
jint Integer::divideUnsigned(jint a, jint b) { const auto y=static_cast<std::uint32_t>(b); if(y==0)throw ArithmeticException("/ by zero"); return static_cast<jint>(static_cast<std::uint32_t>(a)/y); }
jint Integer::remainderUnsigned(jint a, jint b) { const auto y=static_cast<std::uint32_t>(b); if(y==0)throw ArithmeticException("/ by zero"); return static_cast<jint>(static_cast<std::uint32_t>(a)%y); }
jint Integer::highestOneBit(jint v) noexcept { auto x=static_cast<std::uint32_t>(v); if(!x)return 0; return static_cast<jint>(std::uint32_t{1}<<(31-leadingZeros(x))); }
jint Integer::lowestOneBit(jint v) noexcept { const auto x=static_cast<std::uint32_t>(v); return static_cast<jint>(x & (~x + 1U)); }
jint Integer::numberOfLeadingZeros(jint v) noexcept { return leadingZeros(static_cast<std::uint32_t>(v)); }
jint Integer::numberOfTrailingZeros(jint v) noexcept { return trailingZeros(static_cast<std::uint32_t>(v)); }
jint Integer::bitCount(jint v) noexcept { return popcount(static_cast<std::uint32_t>(v)); }
jint Integer::rotateLeft(jint v,jint d) noexcept { const auto x=static_cast<std::uint32_t>(v); const auto n=static_cast<unsigned>(d)&31U; return static_cast<jint>((x<<n)|(x>>((-n)&31U))); }
jint Integer::rotateRight(jint v,jint d) noexcept { return rotateLeft(v,-d); }
jint Integer::reverse(jint v) noexcept { return static_cast<jint>(reverseBits(static_cast<std::uint32_t>(v))); }
jint Integer::signum(jint v) noexcept { return (v>0)-(v<0); }
jint Integer::reverseBytes(jint v) noexcept { auto x=static_cast<std::uint32_t>(v); x=((x&0x00ff00ffU)<<8)|((x>>8)&0x00ff00ffU); return static_cast<jint>((x<<16)|(x>>16)); }
jint Integer::sum(jint a,jint b) noexcept { return static_cast<jint>(static_cast<std::uint32_t>(a)+static_cast<std::uint32_t>(b)); }
jint Integer::min(jint a,jint b) noexcept { return a<b?a:b; }
jint Integer::max(jint a,jint b) noexcept { return a>b?a:b; }

::jxx::Ptr<String> Long::toString(jlong value, jint radix) {
    if (radix < 2 || radix > 36) radix = 10;
    if (value >= 0) return unsignedText(static_cast<std::uint64_t>(value), radix);
    const auto magnitude = std::uint64_t{0} - static_cast<std::uint64_t>(value);
    return ::jxx::NEW<String>("-" + unsignedText(magnitude, radix)->utf8());
}
::jxx::Ptr<String> Long::toUnsignedString(jlong value) { return toUnsignedString(value, 10); }
::jxx::Ptr<String> Long::toUnsignedString(jlong value, jint radix) { return unsignedText(static_cast<std::uint64_t>(value), radix); }
::jxx::Ptr<String> Long::toHexString(jlong value) { return toUnsignedString(value, 16); }
::jxx::Ptr<String> Long::toOctalString(jlong value) { return toUnsignedString(value, 8); }
::jxx::Ptr<String> Long::toBinaryString(jlong value) { return toUnsignedString(value, 2); }
jlong Long::parseUnsignedLong(const ::jxx::Ptr<String>& value) { return parseUnsignedLong(value, 10); }
jlong Long::parseUnsignedLong(const ::jxx::Ptr<String>& value, jint radix) { return static_cast<jlong>(parseUnsigned<std::uint64_t>(value, radix)); }
jint Long::compareUnsigned(jlong a,jlong b) noexcept { const auto x=static_cast<std::uint64_t>(a), y=static_cast<std::uint64_t>(b); return x<y?-1:x>y?1:0; }
jlong Long::divideUnsigned(jlong a,jlong b) { const auto y=static_cast<std::uint64_t>(b); if(y==0)throw ArithmeticException("/ by zero"); return static_cast<jlong>(static_cast<std::uint64_t>(a)/y); }
jlong Long::remainderUnsigned(jlong a,jlong b) { const auto y=static_cast<std::uint64_t>(b); if(y==0)throw ArithmeticException("/ by zero"); return static_cast<jlong>(static_cast<std::uint64_t>(a)%y); }
jlong Long::highestOneBit(jlong v) noexcept { auto x=static_cast<std::uint64_t>(v); if(!x)return 0; return static_cast<jlong>(std::uint64_t{1}<<(63-leadingZeros(x))); }
jlong Long::lowestOneBit(jlong v) noexcept { const auto x=static_cast<std::uint64_t>(v); return static_cast<jlong>(x & (~x + UINT64_C(1))); }
jint Long::numberOfLeadingZeros(jlong v) noexcept { return leadingZeros(static_cast<std::uint64_t>(v)); }
jint Long::numberOfTrailingZeros(jlong v) noexcept { return trailingZeros(static_cast<std::uint64_t>(v)); }
jint Long::bitCount(jlong v) noexcept { return popcount(static_cast<std::uint64_t>(v)); }
jlong Long::rotateLeft(jlong v,jint d) noexcept { const auto x=static_cast<std::uint64_t>(v); const auto n=static_cast<unsigned>(d)&63U; return static_cast<jlong>((x<<n)|(x>>((-n)&63U))); }
jlong Long::rotateRight(jlong v,jint d) noexcept { return rotateLeft(v,-d); }
jlong Long::reverse(jlong v) noexcept { return static_cast<jlong>(reverseBits(static_cast<std::uint64_t>(v))); }
jlong Long::signum(jlong v) noexcept { return (v>0)-(v<0); }
jlong Long::reverseBytes(jlong v) noexcept { auto x=static_cast<std::uint64_t>(v); x=((x&UINT64_C(0x00ff00ff00ff00ff))<<8)|((x>>8)&UINT64_C(0x00ff00ff00ff00ff)); x=((x&UINT64_C(0x0000ffff0000ffff))<<16)|((x>>16)&UINT64_C(0x0000ffff0000ffff)); return static_cast<jlong>((x<<32)|(x>>32)); }
jlong Long::sum(jlong a,jlong b) noexcept { return static_cast<jlong>(static_cast<std::uint64_t>(a)+static_cast<std::uint64_t>(b)); }
jlong Long::min(jlong a,jlong b) noexcept { return a<b?a:b; }
jlong Long::max(jlong a,jlong b) noexcept { return a>b?a:b; }

} // namespace jxx::lang
