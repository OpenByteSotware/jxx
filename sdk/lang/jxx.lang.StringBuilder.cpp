#include <algorithm>
#include <sstream>
#include <locale>
#include "jxx.lang.StringBuilder.h"
#include "jxx.lang.StringBuffer.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.StringIndexOutOfBoundsException.h"
#include "jxx.lang.IllegalArgumentException.h"


namespace jxx::lang {

    static inline std::u16string u16_from_char_array(CharArray a, jint off, jint len) {
        std::u16string out;
        out.resize((std::size_t)len);
        for (jint i = 0; i < len; ++i) out[(std::size_t)i] = (char16_t)(*a)[off + i];
        return out;
    }

    void StringBuilder::throwNPE_() { throw NullPointerException(jxx::NEW<String>("null")); }
    void StringBuilder::throwSIOOBE_() { throw StringIndexOutOfBoundsException(jxx::NEW<String>("String index out of range")); }
    void StringBuilder::throwIAE_(const char* msg) { throw IllegalArgumentException(jxx::NEW<String>(msg)); }

    jxx::Ptr<StringBuilder> StringBuilder::self_() {
        return std::static_pointer_cast<StringBuilder>(this->thisPtr);
    }

    // ---- constructors ----
    StringBuilder::StringBuilder() : value_() {}

    StringBuilder::StringBuilder(jint capacity) : value_() {
        if (capacity < 0) throwIAE_("negative capacity");
        value_.reserve((std::size_t)capacity);
    }

    StringBuilder::StringBuilder(jxx::Ptr<String> str) : value_() {
        if (!str) throwNPE_();
        value_ = str->utf16();
    }

    StringBuilder::StringBuilder(jxx::Ptr<CharSequence> seq) : value_() {
        if (!seq) throwNPE_();
        value_ = toUtf16_(seq);
    }

    // ---- capacity / length ----
    jint StringBuilder::length() const { return (jint)value_.size(); }
    jint StringBuilder::capacity() const { return (jint)value_.capacity(); }

    jint StringBuilder::newCapacity_(jint minCapacity) const {
        jint newCap = (jint)value_.capacity() * 2 + 2;
        if (newCap < minCapacity) newCap = minCapacity;
        if (newCap < 0) newCap = minCapacity;
        return newCap;
    }

    void StringBuilder::ensureCapacityInternal_(jint minCapacity) {
        if (minCapacity <= (jint)value_.capacity()) return;
        value_.reserve((std::size_t)newCapacity_(minCapacity));
    }

    void StringBuilder::ensureCapacity(jint minimumCapacity) {
        if (minimumCapacity > 0) ensureCapacityInternal_(minimumCapacity);
    }

    void StringBuilder::trimToSize() { value_.shrink_to_fit(); }

    void StringBuilder::setLength(jint newLength) {
        if (newLength < 0) throwSIOOBE_();
        if (newLength <= (jint)value_.size()) value_.resize((std::size_t)newLength);
        else {
            ensureCapacityInternal_(newLength);
            value_.resize((std::size_t)newLength, (char16_t)0);
        }
    }

    // ---- CharSequence ----
    jchar StringBuilder::charAt(jint index) const {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        return (jchar)value_[(std::size_t)index];
    }

    void StringBuilder::setCharAt(jint index, jchar ch) {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        value_[(std::size_t)index] = (char16_t)ch;
    }

    jxx::Ptr<CharSequence> StringBuilder::subSequence(jint start, jint end) const {
        return std::static_pointer_cast<CharSequence>(substring(start, end));
    }

    jxx::Ptr<String> StringBuilder::toString() const {
        auto ca = jxx::NEW<CharArrayType>((std::uint32_t)value_.size());
        for (jint i = 0; i < (jint)value_.size(); ++i) (*ca)[i] = (jchar)value_[(std::size_t)i];
        return jxx::NEW<String>(ca);
    }

    // ---- code points ----
    jint StringBuilder::codePointAt(jint index) const {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        char16_t c1 = value_[(std::size_t)index];
        if (isHigh_(c1) && (index + 1) < (jint)value_.size()) {
            char16_t c2 = value_[(std::size_t)(index + 1)];
            if (isLow_(c2)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
        }
        return (jint)c1;
    }

    jint StringBuilder::codePointBefore(jint index) const {
        if (index <= 0 || index > (jint)value_.size()) throwSIOOBE_();
        char16_t c2 = value_[(std::size_t)(index - 1)];
        if (isLow_(c2) && (index - 2) >= 0) {
            char16_t c1 = value_[(std::size_t)(index - 2)];
            if (isHigh_(c1)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
        }
        return (jint)c2;
    }

    jint StringBuilder::codePointCount(jint beginIndex, jint endIndex) const {
        if (beginIndex < 0 || endIndex < beginIndex || endIndex >(jint)value_.size()) throwSIOOBE_();
        jint count = 0;
        for (jint i = beginIndex; i < endIndex; ++i) {
            char16_t c = value_[(std::size_t)i];
            if (isHigh_(c) && (i + 1) < endIndex && isLow_(value_[(std::size_t)(i + 1)])) { ++count; ++i; }
            else ++count;
        }
        return count;
    }

    jint StringBuilder::offsetByCodePoints(jint index, jint codePointOffset) const {
        if (index < 0 || index >(jint)value_.size()) throwSIOOBE_();
        jint i = index;
        if (codePointOffset >= 0) {
            jint r = codePointOffset;
            while (r-- > 0) {
                if (i >= (jint)value_.size()) throwSIOOBE_();
                char16_t c = value_[(std::size_t)i++];
                if (isHigh_(c) && i < (jint)value_.size() && isLow_(value_[(std::size_t)i])) ++i;
            }
            return i;
        }
        else {
            jint r = -codePointOffset;
            while (r-- > 0) {
                if (i <= 0) throwSIOOBE_();
                char16_t c2 = value_[(std::size_t)(--i)];
                if (isLow_(c2) && i > 0 && isHigh_(value_[(std::size_t)(i - 1)])) --i;
            }
            return i;
        }
    }

    jxx::Ptr<StringBuilder> StringBuilder::appendCodePoint(jint codePoint) {
        if (codePoint < 0 || codePoint > 0x10FFFF) throwIAE_("Invalid code point");
        if (codePoint <= 0xFFFF) value_.push_back((char16_t)codePoint);
        else {
            jint cp = codePoint - 0x10000;
            value_.push_back((char16_t)(0xD800 + ((cp >> 10) & 0x3FF)));
            value_.push_back((char16_t)(0xDC00 + (cp & 0x3FF)));
        }
        return self_();
    }

    // ---- utf16 helpers ----
    std::u16string StringBuilder::toUtf16_(jxx::Ptr<CharSequence> s) {
        if (!s) throwNPE_();
        std::u16string out;
        out.reserve((std::size_t)s->length());
        for (jint i = 0; i < s->length(); ++i) out.push_back((char16_t)s->charAt(i));
        return out;
    }

    void StringBuilder::appendUtf16_(const std::u16string& s) {
        if (s.empty()) return;
        ensureCapacityInternal_((jint)value_.size() + (jint)s.size());
        value_.append(s);
    }

    void StringBuilder::insertUtf16_(jint offset, const std::u16string& s) {
        if (offset < 0 || offset >(jint)value_.size()) throwSIOOBE_();
        if (s.empty()) return;
        ensureCapacityInternal_((jint)value_.size() + (jint)s.size());
        value_.insert(value_.begin() + offset, s.begin(), s.end());
    }

    // ---- append overloads ----
    jxx::Ptr<StringBuilder> StringBuilder::append(jbool b) { return append(jxx::NEW<String>(b ? "true" : "false")); }

    // Virtual Appendable::append(jchar) - returns Ptr<Appendable>
    jxx::Ptr<Appendable> StringBuilder::append(jchar c) { value_.push_back((char16_t)c); return self_(); }

    // StringBuilder-specific appendSB(jchar) - returns Ptr<StringBuilder>
    jxx::Ptr<StringBuilder> StringBuilder::appendSB(jchar c) { value_.push_back((char16_t)c); return self_(); }

    jxx::Ptr<StringBuilder> StringBuilder::append(CharArray str) {
        if (!str) return append(jxx::NEW<String>("null"));
        return append(str, 0, (jint)str->length);
    }

    jxx::Ptr<StringBuilder> StringBuilder::append(CharArray str, jint offset, jint len) {
        if (!str) return append(jxx::NEW<String>("null"));
        if (offset < 0 || len < 0 || (std::uint32_t)(offset + len) > str->length) throwSIOOBE_();
        appendUtf16_(u16_from_char_array(str, offset, len));
        return self_();
    }

    // Virtual Appendable::append(CharSequence) - returns Ptr<Appendable>
    jxx::Ptr<Appendable> StringBuilder::append(const jxx::Ptr<CharSequence> s) {
        if (!s) return append(jxx::NEW<String>("null"));
        appendUtf16_(toUtf16_(s));
        return self_();
    }

    // StringBuilder-specific appendSB(CharSequence) - returns Ptr<StringBuilder>
    jxx::Ptr<StringBuilder> StringBuilder::appendSB(const jxx::Ptr<CharSequence> s) {
        if (!s) return append(jxx::NEW<String>("null"));
        appendUtf16_(toUtf16_(s));
        return self_();
    }

    // Virtual Appendable::append(CharSequence, int, int) - returns Ptr<Appendable>
    jxx::Ptr<Appendable> StringBuilder::append(const jxx::Ptr<CharSequence> s, jint start, jint end)
    {
        jxx::Ptr<CharSequence> seq = s;

        if (!seq) {
            seq = jxx::NEW<String>("null");
        }

        if (start < 0 || end < start || end > seq->length()) {
            throwSIOOBE_();
        }

        std::u16string sub;
        sub.reserve((std::size_t)(end - start));

        for (jint i = start; i < end; ++i) {
            sub.push_back((char16_t)seq->charAt(i));
        }

        appendUtf16_(sub);
        return self_();
    }

    // StringBuilder-specific appendSB(CharSequence, int, int) - returns Ptr<StringBuilder>
    jxx::Ptr<StringBuilder> StringBuilder::appendSB(const jxx::Ptr<CharSequence> s, jint start, jint end)
    {
        jxx::Ptr<CharSequence> seq = s;

        if (!seq) {
            seq = jxx::NEW<String>("null");
        }

        if (start < 0 || end < start || end > seq->length()) {
            throwSIOOBE_();
        }

        std::u16string sub;
        sub.reserve((std::size_t)(end - start));

        for (jint i = start; i < end; ++i) {
            sub.push_back((char16_t)seq->charAt(i));
        }

        appendUtf16_(sub);
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::append(jdouble d) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
        return append(jxx::NEW<String>(oss.str().c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jfloat f) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
        return append(jxx::NEW<String>(oss.str().c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jint i) { return append(jxx::NEW<String>(std::to_string(i).c_str())); }
    jxx::Ptr<StringBuilder> StringBuilder::append(jlong lng) { return append(jxx::NEW<String>(std::to_string((long long)lng).c_str())); }

    jxx::Ptr<StringBuilder> StringBuilder::append(jxx::Ptr<Object> obj) {
        if (!obj) return append(jxx::NEW<String>("null"));
        return append(obj->toString());
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jxx::Ptr<String> str) {
        if (!str) str = jxx::NEW<String>("null");
        appendUtf16_(str->utf16());
        return self_();
    }
    jxx::Ptr<StringBuilder> StringBuilder::append(jxx::Ptr<StringBuffer> sb) {
        if (!sb) return append(jxx::NEW<String>("null"));
        return append(sb->toString());
    }

    // ---- delete / replace / reverse ----
    jxx::Ptr<StringBuilder> StringBuilder::delete_(jint start, jint end) {
        if (start < 0) throwSIOOBE_();
        if (end > (jint)value_.size()) end = (jint)value_.size();
        if (start > end) throwSIOOBE_();
        value_.erase(value_.begin() + start, value_.begin() + end);
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::deleteCharAt(jint index) {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        value_.erase(value_.begin() + index);
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::replace(jint start, jint end, jxx::Ptr<String> str) {
        if (start < 0 || start > end || end > (jint)value_.size()) throwSIOOBE_();
        if (!str) str = jxx::NEW<String>("null");
        value_.erase(value_.begin() + start, value_.begin() + end);
        insertUtf16_(start, str->utf16());
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::reverse() {
        std::reverse(value_.begin(), value_.end());
        for (std::size_t i = 0; i + 1 < value_.size(); ++i) {
            char16_t c1 = value_[i];
            char16_t c2 = value_[i + 1];
            if (isLow_(c1) && isHigh_(c2)) {
                value_[i] = c2;
                value_[i + 1] = c1;
                ++i;
            }
        }
        return self_();
    }

    // ---- insert overloads ----
    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jbool b) { return insert(offset, jxx::NEW<String>(b ? "true" : "false")); }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jchar c) {
        if (offset < 0 || offset >(jint)value_.size()) throwSIOOBE_();
        value_.insert(value_.begin() + offset, (char16_t)c);
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, const CharArray str) {
        if (!str) return insert(offset, jxx::NEW<String>("null"));
        return insert(offset, str, 0, (jint)str->length);
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint index, const CharArray str, jint offset, jint len) {
        if (!str) return insert(index, jxx::NEW<String>("null"));
        if (offset < 0 || len < 0 || (std::uint32_t)(offset + len) > str->length) throwSIOOBE_();
        insertUtf16_(index, u16_from_char_array(str, offset, len));
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint dstOffset, const jxx::Ptr<CharSequence> s)
    {
        jxx::Ptr<CharSequence> seq = s;

        if (!seq) {
            seq = jxx::NEW<String>("null");
        }

        insertUtf16_(dstOffset, toUtf16_(seq));
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(
        jint dstOffset,
        const jxx::Ptr<CharSequence> s,
        jint start,
        jint end)
    {
        jxx::Ptr<CharSequence> seq = s;

        if (!seq) {
            seq = jxx::NEW<String>("null");
        }

        if (start < 0 || end < start || end > seq->length()) {
            throwSIOOBE_();
        }

        std::u16string sub;
        sub.reserve((std::size_t)(end - start));

        for (jint i = start; i < end; ++i) {
            sub.push_back((char16_t)seq->charAt(i));
        }

        insertUtf16_(dstOffset, sub);
        return self_();
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jdouble d) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d;
        return insert(offset, jxx::NEW<String>(oss.str().c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jfloat f) {
        std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f;
        return insert(offset, jxx::NEW<String>(oss.str().c_str()));
    }
    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jint i) { return insert(offset, jxx::NEW<String>(std::to_string(i).c_str())); }
    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, jlong l) { return insert(offset, jxx::NEW<String>(std::to_string((long long)l).c_str())); }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, const jxx::Ptr<Object> obj) {
        if (!obj) return insert(offset, jxx::NEW<String>("null"));
        return insert(offset, obj->toString());
    }

    jxx::Ptr<StringBuilder> StringBuilder::insert(jint offset, const jxx::Ptr<String> str)
    {
        jxx::Ptr<String> localStr = str;

        if (!localStr) {
            localStr = jxx::NEW<String>("null");
        }

        insertUtf16_(offset, localStr->utf16());
        return self_();
    }

    // ---- getChars ----
    void StringBuilder::getChars(jint srcBegin, jint srcEnd, const CharArray dst, jint dstBegin) const {
        if (!dst) throwNPE_();
        if (srcBegin < 0 || srcEnd < srcBegin || srcEnd >(jint)value_.size()) throwSIOOBE_();
        if (dstBegin < 0) throwSIOOBE_();
        if ((std::uint32_t)(dstBegin + (srcEnd - srcBegin)) > dst->length) throwSIOOBE_();
        for (jint i = 0; i < (srcEnd - srcBegin); ++i) {
            (*dst)[dstBegin + i] = (jchar)value_[(std::size_t)(srcBegin + i)];
        }
    }

    // ---- indexOf / lastIndexOf ----
    jint StringBuilder::indexOf(const jxx::Ptr<String> str) const { return indexOf(str, 0); }

    jint StringBuilder::indexOf(const jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE_();
        if (fromIndex < 0) fromIndex = 0;
        if (fromIndex > (jint)value_.size()) return -1;
        const auto& needle = str->utf16();
        if (needle.empty()) return fromIndex;
        auto pos = value_.find(needle, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    jint StringBuilder::lastIndexOf(const jxx::Ptr<String> str) const { return lastIndexOf(str, (jint)value_.size()); }

    jint StringBuilder::lastIndexOf(const jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE_();
        const auto& needle = str->utf16();
        if (needle.empty()) return std::min(fromIndex, (jint)value_.size());
        if (fromIndex > (jint)value_.size()) fromIndex = (jint)value_.size();
        if (fromIndex < 0) return -1;
        auto pos = value_.rfind(needle, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    // ---- substring ----
    jxx::Ptr<String> StringBuilder::substring(jint start) const { return substring(start, (jint)value_.size()); }

    jxx::Ptr<String> StringBuilder::substring(jint start, jint end) const {
        if (start < 0 || end < start || end >(jint)value_.size()) throwSIOOBE_();
        jint n = end - start;
        auto ca = jxx::NEW<CharArrayType>((std::uint32_t)n);
        for (jint i = 0; i < n; ++i) (*ca)[i] = (jchar)value_[(std::size_t)(start + i)];
        return jxx::NEW<String>(ca);
    }

} // namespace jxx::lang