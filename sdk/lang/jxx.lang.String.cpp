
#include <algorithm>
#include <sstream>
#include <regex>
#include <locale>
#include <vector>
#include "util/jxx.util.Locale.h"
#include "jxx.lang.NullPointerException.h"
#include "jxx.lang.StringIndexOutOfBoundsException.h"
#include "jxx.lang.IllegalArgumentException.h"
#include "jxx.lang.StringBuffer.h"
#include "jxx.lang.StringBuilder.h"
#include "util/jxx.util.IntStream.h"
#include "util/jxx.util.Formatter.h"
#include "jxx.lang.Cast.h"
#include "jxx.lang.String.h"


namespace jxx::lang {

    String::String(const std::string& stdString) {
        value_ = utf8ToUtf16_(stdString);        
    }

    void String::throwNPE_() { throw NullPointerException(jxx::NEW<String>("null")); }
    void String::throwSIOOBE_() { throw StringIndexOutOfBoundsException(jxx::NEW<String>("String index out of range")); }
    void String::throwIAE_(const char* msg) { throw IllegalArgumentException(jxx::NEW<String>(msg)); }

    char16_t String::toLowerAscii_(char16_t c) {
        if (c >= u'A' && c <= u'Z') return (char16_t)(c - u'A' + u'a');
        return c;
    }
    char16_t String::toUpperAscii_(char16_t c) {
        if (c >= u'a' && c <= u'z') return (char16_t)(c - u'a' + u'A');
        return c;
    }

    bool String::isTurkicLocale_(jxx::Ptr<jxx::util::Locale> loc) {
        if (!loc) return false;
        auto lang = loc->getLanguage();
        if (!lang) return false;
        auto s = lang->utf8();
        return (s == "tr" || s == "az");
    }

    std::u16string String::toUtf16_(jxx::Ptr<CharSequence> s) {
        if (!s) throwNPE_();
        std::u16string out;
        out.reserve((std::size_t)s->length());
        for (jint i = 0; i < s->length(); ++i) out.push_back((char16_t)s->charAt(i));
        return out;
    }

    // UTF-8 helpers (used only for regex via std::regex; charset encode/decode uses Charset)
    std::u16string String::utf8ToUtf16_(const std::string& s) {
        std::u16string out;
        out.reserve(s.size());

        const unsigned char* p = (const unsigned char*)s.data();
        const unsigned char* e = p + s.size();

        while (p < e) {
            uint32_t cp = 0;
            unsigned char c = *p++;

            if (c < 0x80) cp = c;
            else if ((c >> 5) == 0x6 && p < e) {
                cp = (uint32_t)(c & 0x1F) << 6;
                cp |= (uint32_t)(*p++ & 0x3F);
            }
            else if ((c >> 4) == 0xE && p + 1 < e) {
                cp = (uint32_t)(c & 0x0F) << 12;
                cp |= (uint32_t)(*p++ & 0x3F) << 6;
                cp |= (uint32_t)(*p++ & 0x3F);
            }
            else if ((c >> 3) == 0x1E && p + 2 < e) {
                cp = (uint32_t)(c & 0x07) << 18;
                cp |= (uint32_t)(*p++ & 0x3F) << 12;
                cp |= (uint32_t)(*p++ & 0x3F) << 6;
                cp |= (uint32_t)(*p++ & 0x3F);
            }
            else cp = 0xFFFD;

            if (cp <= 0xFFFF) out.push_back((char16_t)cp);
            else {
                cp -= 0x10000;
                out.push_back((char16_t)(0xD800 + ((cp >> 10) & 0x3FF)));
                out.push_back((char16_t)(0xDC00 + (cp & 0x3FF)));
            }
        }
        return out;
    }

    std::string String::utf16ToUtf8_(const std::u16string& s) {
        std::string out;
        out.reserve(s.size());

        for (size_t i = 0; i < s.size(); ++i) {
            uint32_t cp = s[i];
            if (isHigh_(s[i]) && i + 1 < s.size() && isLow_(s[i + 1])) {
                uint32_t hi = (uint32_t)s[i++] - 0xD800;
                uint32_t lo = (uint32_t)s[i] - 0xDC00;
                cp = (hi << 10) + lo + 0x10000;
            }

            if (cp < 0x80) out.push_back((char)cp);
            else if (cp < 0x800) {
                out.push_back((char)(0xC0 | (cp >> 6)));
                out.push_back((char)(0x80 | (cp & 0x3F)));
            }
            else if (cp < 0x10000) {
                out.push_back((char)(0xE0 | (cp >> 12)));
                out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
                out.push_back((char)(0x80 | (cp & 0x3F)));
            }
            else {
                out.push_back((char)(0xF0 | (cp >> 18)));
                out.push_back((char)(0x80 | ((cp >> 12) & 0x3F)));
                out.push_back((char)(0x80 | ((cp >> 6) & 0x3F)));
                out.push_back((char)(0x80 | (cp & 0x3F)));
            }
        }
        return out;
    }

    std::string String::utf8() const { return utf16ToUtf8_(value_); }

    // ---------------- Constructors ----------------
    String::String() : value_() {}

    String::String(jxx::Ptr<String> original) {
        if (!original) throwNPE_();
        value_ = original->value_;
    }

    String::String(const char* utf8c) {
        if (!utf8c) { value_.clear(); return; }
        value_ = utf8ToUtf16_(std::string(utf8c));
    }

    // byte[] ctors use Charset (Java default charset)
    String::String(const ByteArray bytes) : String(bytes, 0, bytes ? (jint)bytes->length : 0) {}

    String::String(const ByteArray bytes, jint offset, jint length) {
        if (!bytes) throwNPE_();
        if (offset < 0 || length < 0 || (std::uint32_t)(offset + length) > bytes->length) throwSIOOBE_();

        auto slice = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)length);
        for (jint i = 0; i < length; ++i) (*slice)[i] = (*bytes)[offset + i];

        auto cs = Charset::defaultCharset();
        value_ = cs->decode(slice)->utf16();
    }

    String::String(const ByteArray bytes, jxx::Ptr<String> charsetName) {
        if (!bytes || !charsetName) throwNPE_();
        auto cs = Charset::forName(charsetName);
        value_ = cs->decode(bytes)->utf16();
    }

    String::String(const ByteArray bytes, jint offset, jint length, jxx::Ptr<String> charsetName) {
        if (!bytes || !charsetName) throwNPE_();
        if (offset < 0 || length < 0 || (std::uint32_t)(offset + length) > bytes->length) throwSIOOBE_();

        auto slice = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)length);
        for (jint i = 0; i < length; ++i) (*slice)[i] = (*bytes)[offset + i];

        auto cs = Charset::forName(charsetName);
        value_ = cs->decode(slice)->utf16();
    }

    String::String(const ByteArray bytes, jxx::Ptr<Charset> charset) {
        if (!bytes || !charset) throwNPE_();
        value_ = charset->decode(bytes)->utf16();
    }

    String::String(const ByteArray bytes, jint offset, jint length, jxx::Ptr<Charset> charset) {
        if (!bytes || !charset) throwNPE_();
        if (offset < 0 || length < 0 || (std::uint32_t)(offset + length) > bytes->length) throwSIOOBE_();

        auto slice = jxx::NEW<jxx::lang::ByteArrayType>((std::uint32_t)length);
        for (jint i = 0; i < length; ++i) (*slice)[i] = (*bytes)[offset + i];

        value_ = charset->decode(slice)->utf16();
    }

    // deprecated hibyte constructors
    String::String(const ByteArray ascii, jint hibyte) : String(ascii, hibyte, 0, ascii ? (jint)ascii->length : 0) {}

    String::String(const ByteArray ascii, jint hibyte, jint offset, jint count) {
        if (!ascii) throwNPE_();
        if (offset < 0 || count < 0 || (std::uint32_t)(offset + count) > ascii->length) throwSIOOBE_();
        value_.resize((size_t)count);
        for (jint i = 0; i < count; ++i) {
            unsigned char b = (unsigned char)(*ascii)[offset + i];
            value_[(size_t)i] = (char16_t)((hibyte << 8) | b);
        }
    }

    // char[] constructors
    String::String(const CharArray value) : String(value, 0, value ? (jint)value->length : 0) {}

    String::String(const CharArray value, jint offset, jint count) {
        if (!value) throwNPE_();
        if (offset < 0 || count < 0 || (std::uint32_t)(offset + count) > value->length) throwSIOOBE_();
        value_.resize((size_t)count);
        for (jint i = 0; i < count; ++i) value_[(size_t)i] = (char16_t)(*value)[offset + i];
    }

    // codePoints constructor
    String::String(const IntArray cps, jint offset, jint count) {
        if (!cps) throwNPE_();
        if (offset < 0 || count < 0 || (std::uint32_t)(offset + count) > cps->length) throwSIOOBE_();
        value_.clear();
        for (jint i = 0; i < count; ++i) {
            jint cp = (*cps)[offset + i];
            if (cp < 0 || cp > 0x10FFFF) cp = 0xFFFD;
            if (cp <= 0xFFFF) value_.push_back((char16_t)cp);
            else {
                cp -= 0x10000;
                value_.push_back((char16_t)(0xD800 + ((cp >> 10) & 0x3FF)));
                value_.push_back((char16_t)(0xDC00 + (cp & 0x3FF)));
            }
        }
    }

    // StringBuffer/StringBuilder ctors depend on your classes existing
    String::String(const jxx::Ptr<StringBuffer> buffer) { if (!buffer) throwNPE_(); value_ = buffer->toString()->utf16(); }
    String::String(const jxx::Ptr<StringBuilder> builder) { if (!builder) throwNPE_(); value_ = builder->toString()->utf16(); }

    // ---------------- CharSequence ----------------
    jint String::length() const { return (jint)value_.size(); }

    jchar String::charAt(jint index) const {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        return (jchar)value_[(size_t)index];
    }

    jxx::Ptr<CharSequence> String::subSequence(jint start, jint end) const {
        return std::static_pointer_cast<CharSequence>(substring(start, end));
    }

    jxx::Ptr<String> String::toString() const {
        return std::static_pointer_cast<String>(this->thisPtr);
    }
    // Comparable
    jint String::compareTo(const jxx::Ptr<String> another) const {
        if (!another) throwNPE_();
        const auto& b = another->value_;
        size_t n = std::min(value_.size(), b.size());
        for (size_t i = 0; i < n; ++i) {
            if (value_[i] != b[i]) return (jint)value_[i] - (jint)b[i];
        }
        return (jint)value_.size() - (jint)b.size();
    }

    // Object
    jbool String::equals(const jxx::Ptr<Object> obj) const {
        auto s = std::dynamic_pointer_cast<String>(obj);
        return s && s->value_ == value_;
    }

    jint String::hashCode() const {
        if (hashComputed_) return hash_;
        jint h = 0;
        for (char16_t c : value_) h = 31 * h + (jint)c;
        hash_ = h;
        hashComputed_ = true;
        return h;
    }

    // ---------- remaining methods (full) ----------
    jbool String::isEmpty() const { return value_.empty(); }

    jint String::codePointAt(jint index) const {
        if (index < 0 || index >= (jint)value_.size()) throwSIOOBE_();
        char16_t c1 = value_[(size_t)index];
        if (isHigh_(c1) && (index + 1) < (jint)value_.size()) {
            char16_t c2 = value_[(size_t)(index + 1)];
            if (isLow_(c2)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
        }
        return (jint)c1;
    }

    jint String::codePointBefore(jint index) const {
        if (index <= 0 || index > (jint)value_.size()) throwSIOOBE_();
        char16_t c2 = value_[(size_t)(index - 1)];
        if (isLow_(c2) && index - 2 >= 0) {
            char16_t c1 = value_[(size_t)(index - 2)];
            if (isHigh_(c1)) return (((jint)c1 - 0xD800) << 10) + ((jint)c2 - 0xDC00) + 0x10000;
        }
        return (jint)c2;
    }

    jint String::codePointCount(jint beginIndex, jint endIndex) const {
        if (beginIndex < 0 || endIndex < beginIndex || endIndex >(jint)value_.size()) throwSIOOBE_();
        jint count = 0;
        for (jint i = beginIndex; i < endIndex; ++i) {
            char16_t c = value_[(size_t)i];
            if (isHigh_(c) && i + 1 < endIndex && isLow_(value_[(size_t)(i + 1)])) { ++count; ++i; }
            else ++count;
        }
        return count;
    }

    jint String::offsetByCodePoints(jint index, jint codePointOffset) const {
        if (index < 0 || index >(jint)value_.size()) throwSIOOBE_();
        jint i = index;
        if (codePointOffset >= 0) {
            jint r = codePointOffset;
            while (r-- > 0) {
                if (i >= (jint)value_.size()) throwSIOOBE_();
                char16_t c = value_[(size_t)i++];
                if (isHigh_(c) && i < (jint)value_.size() && isLow_(value_[(size_t)i])) ++i;
            }
            return i;
        }
        else {
            jint r = -codePointOffset;
            while (r-- > 0) {
                if (i <= 0) throwSIOOBE_();
                char16_t c2 = value_[(size_t)(--i)];
                if (isLow_(c2) && i > 0 && isHigh_(value_[(size_t)(i - 1)])) --i;
            }
            return i;
        }
    }

    void String::getChars(jint srcBegin, jint srcEnd, const CharArray dst, jint dstBegin) const {
        if (!dst) throwNPE_();
        if (srcBegin < 0 || srcEnd < srcBegin || srcEnd >(jint)value_.size()) throwSIOOBE_();
        if (dstBegin < 0) throwSIOOBE_();
        if ((std::uint32_t)(dstBegin + (srcEnd - srcBegin)) > dst.get()->length) throwSIOOBE_();
        for (jint i = 0; i < (srcEnd - srcBegin); ++i) (*dst)[dstBegin + i] = (jchar)value_[(size_t)(srcBegin + i)];
    }

    void String::getBytes(jint srcBegin, jint srcEnd, const ByteArray dst, jint dstBegin) const {
        if (!dst) throwNPE_();
        if (srcBegin < 0 || srcEnd < srcBegin || srcEnd >(jint)value_.size()) throwSIOOBE_();
        if (dstBegin < 0) throwSIOOBE_();
        if ((std::uint32_t)(dstBegin + (srcEnd - srcBegin)) > dst.get()->length) throwSIOOBE_();
        for (jint i = 0; i < (srcEnd - srcBegin); ++i) (*dst)[dstBegin + i] = (jbyte)(value_[(size_t)(srcBegin + i)] & 0xFF);
    }

    ByteArray String::getBytes() const {
        auto cs = Charset::defaultCharset();
        return cs->encode(std::static_pointer_cast<String>(this->thisPtr));
    }

    ByteArray String::getBytes(jxx::Ptr<String> charsetName) const {
        if (!charsetName) throwNPE_();
        auto cs = Charset::forName(charsetName);
        return cs->encode(std::static_pointer_cast<String>(this->thisPtr));
    }

    ByteArray String::getBytes(jxx::Ptr<Charset> charset) const {
        if (!charset) throwNPE_();
        return charset->encode(std::static_pointer_cast<String>(this->thisPtr));
    }

    jbool String::contentEquals(jxx::Ptr<CharSequence> cs) const {
        if (!cs) return false;
        if (cs->length() != length()) return false;
        for (jint i = 0; i < length(); ++i) if (cs->charAt(i) != charAt(i)) return false;
        return true;
    }

    jbool String::contentEquals(jxx::Ptr<StringBuffer> sb) const {
        if (!sb) return false;
        return contentEquals(std::static_pointer_cast<CharSequence>(sb->toString()));
    }

    jbool String::equalsIgnoreCase(jxx::Ptr<String> other) const {
        if (!other) return false;
        if (other->length() != length()) return false;
        for (jint i = 0; i < length(); ++i)
            if (toLowerAscii_(value_[(size_t)i]) != toLowerAscii_(other->value_[(size_t)i])) return false;
        return true;
    }

    jint String::compareToIgnoreCase(jxx::Ptr<String> other) const {
        if (!other) throwNPE_();
        size_t n = std::min(value_.size(), other->value_.size());
        for (size_t i = 0; i < n; ++i) {
            char16_t a = toLowerAscii_(value_[i]);
            char16_t b = toLowerAscii_(other->value_[i]);
            if (a != b) return (jint)a - (jint)b;
        }
        return (jint)value_.size() - (jint)other->value_.size();
    }

    jbool String::regionMatches(jint toffset, jxx::Ptr<String> other, jint ooffset, jint len) const {
        return regionMatches(false, toffset, other, ooffset, len);
    }

    jbool String::regionMatches(jbool ignoreCase, jint toffset, jxx::Ptr<String> other, jint ooffset, jint len) const {
        if (!other) throwNPE_();
        if (toffset < 0 || ooffset < 0 || len < 0) return false;
        if (toffset + len > length()) return false;
        if (ooffset + len > other->length()) return false;

        for (jint i = 0; i < len; ++i) {
            char16_t a = value_[(size_t)(toffset + i)];
            char16_t b = other->value_[(size_t)(ooffset + i)];
            if (ignoreCase) { if (toLowerAscii_(a) != toLowerAscii_(b)) return false; }
            else { if (a != b) return false; }
        }
        return true;
    }

    jbool String::startsWith(jxx::Ptr<String> prefix) const { return startsWith(prefix, 0); }

    jbool String::startsWith(jxx::Ptr<String> prefix, jint toffset) const {
        if (!prefix) throwNPE_();
        if (toffset < 0) return false;
        if (toffset + prefix->length() > length()) return false;
        for (jint i = 0; i < prefix->length(); ++i)
            if (value_[(size_t)(toffset + i)] != prefix->value_[(size_t)i]) return false;
        return true;
    }

    jbool String::endsWith(jxx::Ptr<String> suffix) const {
        if (!suffix) throwNPE_();
        return startsWith(suffix, length() - suffix->length());
    }

    jint String::indexOf(jint ch) const { return indexOf(ch, 0); }

    jint String::indexOf(jint ch, jint fromIndex) const {
        if (fromIndex < 0) fromIndex = 0;
        if (fromIndex >= length()) return -1;
        char16_t c = (char16_t)ch;
        for (jint i = fromIndex; i < length(); ++i) if (value_[(size_t)i] == c) return i;
        return -1;
    }

    jint String::lastIndexOf(jint ch) const { return lastIndexOf(ch, length() - 1); }

    jint String::lastIndexOf(jint ch, jint fromIndex) const {
        char16_t c = (char16_t)ch;
        if (fromIndex >= length()) fromIndex = length() - 1;
        for (jint i = fromIndex; i >= 0; --i) if (value_[(size_t)i] == c) return i;
        return -1;
    }

    jint String::indexOf(jxx::Ptr<String> str) const { return indexOf(str, 0); }

    jint String::indexOf(jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE_();
        if (fromIndex < 0) fromIndex = 0;
        if (str->value_.empty()) return std::min(fromIndex, length());
        auto pos = value_.find(str->value_, (size_t)fromIndex);
        return pos == std::u16string::npos ? -1 : (jint)pos;
    }

    jint String::lastIndexOf(jxx::Ptr<String> str) const { return lastIndexOf(str, length()); }

    jint String::lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE_();
        if (str->value_.empty()) return std::min(fromIndex, length());
        if (fromIndex > length()) fromIndex = length();
        auto pos = value_.rfind(str->value_, (size_t)fromIndex);
        return pos == std::u16string::npos ? -1 : (jint)pos;
    }

    jxx::Ptr<String> String::substring(jint beginIndex) const { return substring(beginIndex, length()); }

    jxx::Ptr<String> String::substring(jint beginIndex, jint endIndex) const {
        if (beginIndex < 0 || endIndex < beginIndex || endIndex > length()) throwSIOOBE_();
        auto out = jxx::NEW<String>();
        out->value_ = value_.substr((size_t)beginIndex, (size_t)(endIndex - beginIndex));
        return out;
    }

    jxx::Ptr<String> String::concat(jxx::Ptr<String> str) const {
        if (!str) throwNPE_();
        if (str->value_.empty()) return std::static_pointer_cast<String>(this->thisPtr);
        auto out = jxx::NEW<String>();
        out->value_.reserve(value_.size() + str->value_.size());
        out->value_ = value_;
        out->value_.append(str->value_);
        return out;
    }

    jxx::Ptr<String> String::replace(jchar oldChar, jchar newChar) const {
        auto out = jxx::NEW<String>();
        out->value_ = value_;
        for (auto& c : out->value_) if (c == (char16_t)oldChar) c = (char16_t)newChar;
        return out;
    }

    jbool String::matches(jxx::Ptr<String> regex) const {
        if (!regex) throwNPE_();
        std::regex re(regex->utf8());
        return std::regex_match(utf8(), re);
    }

    jbool String::contains(jxx::Ptr<CharSequence> s) const {
        if (!s) throwNPE_();
        auto t = toUtf16_(s);
        if (t.empty()) return true;
        return value_.find(t) != std::u16string::npos;
    }

    jxx::Ptr<String> String::replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const {
        if (!regex || !replacement) throwNPE_();
        std::regex re(regex->utf8());
        std::string out = std::regex_replace(utf8(), re, replacement->utf8(),
            std::regex_constants::format_first_only);
        return jxx::NEW<String>(out.c_str());
    }

    jxx::Ptr<String> String::replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const {
        if (!regex || !replacement) throwNPE_();
        std::regex re(regex->utf8());
        std::string out = std::regex_replace(utf8(), re, replacement->utf8());
        return jxx::NEW<String>(out.c_str());
    }

    jxx::Ptr<String> String::replace(jxx::Ptr<CharSequence> target, jxx::Ptr<CharSequence> replacement) const {
        if (!target || !replacement) throwNPE_();
        std::u16string t = toUtf16_(target);
        std::u16string r = toUtf16_(replacement);
        if (t.empty()) return std::static_pointer_cast<String>(this->thisPtr);

        auto out = jxx::NEW<String>();
        out->value_ = value_;
        size_t pos = 0;
        while ((pos = out->value_.find(t, pos)) != std::u16string::npos) {
            out->value_.replace(pos, t.size(), r);
            pos += r.size();
        }
        return out;
    }

    jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> String::split(jxx::Ptr<String> regex) const {
        return split(regex, 0);
    }

    jxx::Ptr<JxxArray<jxx::Ptr<String>, 1>> String::split(jxx::Ptr<String> regex, jint limit) const {
        if (!regex) throwNPE_();
        std::regex re(regex->utf8());
        std::string s = utf8();

        std::vector<std::string> parts;
        std::sregex_token_iterator it(s.begin(), s.end(), re, -1);
        std::sregex_token_iterator end;

        for (; it != end; ++it) {
            parts.push_back(it->str());
            if (limit > 0 && (jint)parts.size() >= limit) break;
        }

        auto arr = jxx::NEW<JxxArray<jxx::Ptr<String>, 1>>((std::uint32_t)parts.size());
        for (jint i = 0; i < (jint)parts.size(); ++i) (*arr)[i] = jxx::NEW<String>(parts[(size_t)i].c_str());
        return arr;
    }

    jxx::Ptr<String> String::toLowerCase() const {
        return toLowerCase(jxx::util::Locale::getDefault());
    }

    jxx::Ptr<String> String::toLowerCase(jxx::Ptr<jxx::util::Locale> locale) const {
        auto out = jxx::NEW<String>();
        out->value_ = value_;
        bool turkic = isTurkicLocale_(locale);

        for (auto& c : out->value_) {
            if (turkic) {
                if (c == u'I') { c = u'\u0131'; continue; }
                if (c == u'\u0130') { c = u'i'; continue; }
            }
            c = toLowerAscii_(c);
        }
        return out;
    }

    jxx::Ptr<String> String::toUpperCase() const {
        return toUpperCase(jxx::util::Locale::getDefault());
    }

    jxx::Ptr<String> String::toUpperCase(jxx::Ptr<jxx::util::Locale> locale) const {
        auto out = jxx::NEW<String>();
        out->value_ = value_;
        bool turkic = isTurkicLocale_(locale);

        for (auto& c : out->value_) {
            if (turkic) {
                if (c == u'i') { c = u'\u0130'; continue; }
                if (c == u'\u0131') { c = u'I'; continue; }
            }
            c = toUpperAscii_(c);
        }
        return out;
    }

    jxx::Ptr<String> String::trim() const {
        size_t start = 0;
        while (start < value_.size() && value_[start] <= u' ') ++start;
        size_t end = value_.size();
        while (end > start && value_[end - 1] <= u' ') --end;
        auto out = jxx::NEW<String>();
        out->value_ = value_.substr(start, end - start);
        return out;
    }

    CharArray String::toCharArray() const {
        auto a = jxx::NEW<CharArrayType>((std::uint32_t)value_.size());
        for (jint i = 0; i < (jint)value_.size(); ++i) (*a)[i] = (jchar)value_[(size_t)i];
        return a;
    }

    jxx::Ptr<jxx::util::IntStream> String::chars() const {
        auto a = jxx::NEW<IntArrayType>((std::uint32_t)value_.size());
        for (jint i = 0; i < (jint)value_.size(); ++i) (*a)[i] = (jint)value_[(size_t)i];
        return jxx::util::IntStream::of(a);
    }

    jxx::Ptr<jxx::util::IntStream> String::codePoints() const {
        std::vector<jint> cps;
        cps.reserve(value_.size());
        for (size_t i = 0; i < value_.size(); ++i) {
            uint32_t cp = value_[i];
            if (isHigh_((char16_t)cp) && i + 1 < value_.size() && isLow_((char16_t)value_[i + 1])) {
                uint32_t hi = (uint32_t)value_[i++] - 0xD800;
                uint32_t lo = (uint32_t)value_[i] - 0xDC00;
                cp = (hi << 10) + lo + 0x10000;
            }
            cps.push_back((jint)cp);
        }
        auto a = jxx::NEW<IntArrayType>((std::uint32_t)cps.size());
        for (jint i = 0; i < (jint)cps.size(); ++i) (*a)[i] = cps[(size_t)i];
        return jxx::util::IntStream::of(a);
    }

    jxx::Ptr<String> String::intern() const {
        std::lock_guard<std::mutex> lk(internMutex_);
        auto it = internPool_.find(value_);
        if (it != internPool_.end()) {
            if (auto existing = it->second.lock()) return existing;
        }
        auto me = std::static_pointer_cast<String>(this->thisPtr);
        internPool_[value_] = me;
        return me;
    }

    // ---- static methods ----
    jxx::Ptr<String> String::valueOf(jbool b) { return jxx::NEW<String>(b ? "true" : "false"); }
    jxx::Ptr<String> String::valueOf(jchar c) {
        auto a = jxx::NEW<CharArrayType>((std::uint32_t)1);
        (*a)[0] = c;
        return jxx::NEW<String>(a);
    }
    jxx::Ptr<String> String::valueOf(jint i) { return jxx::NEW<String>(std::to_string(i).c_str()); }
    jxx::Ptr<String> String::valueOf(jlong l) { return jxx::NEW<String>(std::to_string((long long)l).c_str()); }
    jxx::Ptr<String> String::valueOf(jfloat f) { std::ostringstream oss; oss.imbue(std::locale::classic()); oss << f; return jxx::NEW<String>(oss.str().c_str()); }
    jxx::Ptr<String> String::valueOf(jdouble d) { std::ostringstream oss; oss.imbue(std::locale::classic()); oss << d; return jxx::NEW<String>(oss.str().c_str()); }
    jxx::Ptr<String> String::valueOf(jxx::Ptr<Object> obj) { return jxx::CAST<String, Object>(obj) ? obj->toString() : jxx::NEW<String>("null"); }
    jxx::Ptr<String> String::valueOf(CharArray data) { return jxx::NEW<String>(data); }
    jxx::Ptr<String> String::valueOf(CharArray data, jint offset, jint count) { return jxx::NEW<String>(data, offset, count); }
    jxx::Ptr<String> String::copyValueOf(CharArray data) { return jxx::NEW<String>(data); }
    jxx::Ptr<String> String::copyValueOf(CharArray data, jint offset, jint count) { return jxx::NEW<String>(data, offset, count); }

    jxx::Ptr<String> String::format(jxx::Ptr<String> format, jxx::Ptr<JxxArray<jxx::Ptr<Object>, 1>> args) {
        if (!format || !args) throwNPE_();
        jxx::util::Formatter f;
        return f.format(format, args)->toString();
    }

    jxx::Ptr<jxx::lang::String> String::format(jxx::Ptr<jxx::util::Locale> l, jxx::Ptr<String> format, jxx::Ptr<JxxArray<jxx::Ptr<Object>, 1>> args) {
        if (!l || !format || !args) throwNPE_();
        jxx::util::Formatter f(l);
        return f.format(format, args)->toString();
    }

    jxx::Ptr<String> String::join(jxx::Ptr<CharSequence> delimiter, jxx::Ptr<JxxArray<jxx::Ptr<CharSequence>, 1>> elements) {
        if (!delimiter || !elements) throwNPE_();
        std::u16string delim = toUtf16_(delimiter);

        std::u16string out;
        for (std::uint32_t i = 0; i < elements->length; ++i) {
            if (i) out.append(delim);
            auto e = (*elements)[(jint)i];
            out.append(e ? toUtf16_(e) : std::u16string(u"null"));
        }

        auto s = jxx::NEW<String>();
        s->value_ = std::move(out);
        return s;
    }

    jxx::Ptr<String> String::join(jxx::Ptr<CharSequence> delimiter,
        jxx::Ptr<jxx::lang::Iterable<jxx::Ptr<CharSequence>>> elements) {

        if (!delimiter || !elements) throwNPE_();

        auto it = elements->iterator();
        if (!it) throwNPE_();

        std::u16string delim = toUtf16_(delimiter);
        std::u16string out;

        bool first = true;
        while (it->hasNext()) {
            if (!first) out.append(delim);
            first = false;

            auto e = it->next(); // jxx::Ptr<CharSequence>
            out.append(e ? toUtf16_(e) : std::u16string(u"null"));
        }

        auto s = jxx::NEW<String>();
        s->value_ = std::move(out);
        return s;
    }

    void String::writeObject(jxx::Ptr<jxx::io::ObjectOutputStream> out) {}
    void String::readObject(jxx::Ptr<jxx::io::ObjectInputStream> in) {}
    void String::readObjectNoData() {}

} // namespace jxx::lang