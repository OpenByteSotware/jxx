#include "jxx.lang.String.h"

#include "jxx.lang.Charset.h"
#include "jxx.lang.Locale.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <regex>
#include <sstream>
#include <stdexcept>

namespace jxx::lang {

    // =========================
    // Intern pool
    // =========================
    std::mutex& String::internMutex() {
        static std::mutex m;
        return m;
    }

    std::unordered_map<std::u16string, std::weak_ptr<String>>& String::internPool() {
        static std::unordered_map<std::u16string, std::weak_ptr<String>> pool;
        return pool;
    }

    // =========================
    // Surrogate helpers
    // =========================
    bool String::isHighSurrogate(char16_t c) { return c >= 0xD800 && c <= 0xDBFF; }
    bool String::isLowSurrogate(char16_t c) { return c >= 0xDC00 && c <= 0xDFFF; }

    jint String::toCodePoint(char16_t high, char16_t low) {
        return (((jint)high - 0xD800) << 10) + ((jint)low - 0xDC00) + 0x10000;
    }

    // =========================
    // Exceptions (replace with your own types if desired)
    // =========================
    void String::throwNPE() {
        throw std::invalid_argument("NullPointerException");
    }
    void String::throwSIOOBE() {
        throw std::out_of_range("StringIndexOutOfBoundsException");
    }

    // =========================
    // UTF-16 <-> UTF-8 conversion
    // (Minimal, correct for valid UTF-16; replaces malformed sequences with U+FFFD)
    // =========================
    static std::string utf16_to_utf8(const std::u16string& s) {
        std::string out;
        out.reserve(s.size()); // lower bound
        for (std::size_t i = 0; i < s.size(); ++i) {
            uint32_t cp = (uint32_t)s[i];
            if (String::isHighSurrogate((char16_t)cp) && (i + 1) < s.size() && String::isLowSurrogate(s[i + 1])) {
                cp = (uint32_t)String::toCodePoint(s[i], s[i + 1]);
                ++i;
            }
            else if (String::isHighSurrogate((char16_t)cp) || String::isLowSurrogate((char16_t)cp)) {
                cp = 0xFFFD; // replacement char for malformed surrogate
            }

            if (cp < 0x80) {
                out.push_back((char)cp);
            }
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

    static std::u16string utf8_to_utf16(const std::string& s) {
        std::u16string out;
        out.reserve(s.size());

        std::size_t i = 0;
        while (i < s.size()) {
            uint32_t cp = 0;
            unsigned char c = (unsigned char)s[i];

            if (c < 0x80) {
                cp = c;
                i += 1;
            }
            else if ((c >> 5) == 0x6 && i + 1 < s.size()) {
                cp = ((c & 0x1F) << 6) | ((unsigned char)s[i + 1] & 0x3F);
                i += 2;
            }
            else if ((c >> 4) == 0xE && i + 2 < s.size()) {
                cp = ((c & 0x0F) << 12) |
                    (((unsigned char)s[i + 1] & 0x3F) << 6) |
                    ((unsigned char)s[i + 2] & 0x3F);
                i += 3;
            }
            else if ((c >> 3) == 0x1E && i + 3 < s.size()) {
                cp = ((c & 0x07) << 18) |
                    (((unsigned char)s[i + 1] & 0x3F) << 12) |
                    (((unsigned char)s[i + 2] & 0x3F) << 6) |
                    ((unsigned char)s[i + 3] & 0x3F);
                i += 4;
            }
            else {
                cp = 0xFFFD;
                i += 1;
            }

            if (cp <= 0xFFFF) {
                out.push_back((char16_t)cp);
            }
            else {
                cp -= 0x10000;
                char16_t high = (char16_t)(0xD800 + ((cp >> 10) & 0x3FF));
                char16_t low = (char16_t)(0xDC00 + (cp & 0x3FF));
                out.push_back(high);
                out.push_back(low);
            }
        }

        return out;
    }

    // =========================
    // Constructors
    // =========================
    String::String() : data_() {}

    String::String(const char* utf8) : data_() {
        if (!utf8) { data_.clear(); return; }
        data_ = utf8_to_utf16(std::string(utf8));
    }

    String::String(jxx::Ptr<String> other) : data_() {
        if (!other) throwNPE();
        data_ = other->data_;
    }

    String::String(jxx::Ptr<CharArray> value) : data_() {
        if (!value) throwNPE();
        data_.resize(value->length);
        for (std::uint32_t i = 0; i < value->length; ++i) {
            data_[i] = (char16_t)(*value)[(jint)i];
        }
    }

    String::String(jxx::Ptr<CharArray> value, jint offset, jint count) : data_() {
        if (!value) throwNPE();
        if (offset < 0 || count < 0) throwSIOOBE();
        if ((std::uint32_t)(offset + count) > value->length) throwSIOOBE();

        data_.resize((std::size_t)count);
        for (jint i = 0; i < count; ++i) {
            data_[(std::size_t)i] = (char16_t)(*value)[offset + i];
        }
    }

    // =========================
    // CharSequence
    // =========================
    jint String::length() const {
        return (jint)data_.size();
    }

    jchar String::charAt(jint index) const {
        if (index < 0 || index >= (jint)data_.size()) throwSIOOBE();
        return (jchar)data_[(std::size_t)index];
    }

    jxx::Ptr<CharSequence> String::subSequence(jint beginIndex, jint endIndex) const {
        return substring(beginIndex, endIndex);
    }

    jxx::Ptr<String> String::toString() const {
        // Java returns itself
        return std::const_pointer_cast<String>(Object::shared_from_this());
    }

    // =========================
    // Code point APIs
    // =========================
    jint String::codePointAt(jint index) const {
        if (index < 0 || index >= (jint)data_.size()) String::throwSIOOBE();
        char16_t c1 = data_[(std::size_t)index];
        if (isHighSurrogate(c1) && (index + 1) < (jint)data_.size()) {
            char16_t c2 = data_[(std::size_t)(index + 1)];
            if (isLowSurrogate(c2)) return toCodePoint(c1, c2);
        }
        return (jint)c1;
    }

    jint String::codePointBefore(jint index) const {
        if (index <= 0 || index > (jint)data_.size()) throwSIOOBE();
        char16_t c2 = data_[(std::size_t)(index - 1)];
        if (isLowSurrogate(c2) && (index - 2) >= 0) {
            char16_t c1 = data_[(std::size_t)(index - 2)];
            if (isHighSurrogate(c1)) return toCodePoint(c1, c2);
        }
        return (jint)c2;
    }

    jint String::codePointCount(jint beginIndex, jint endIndex) const {
        if (beginIndex < 0 || endIndex < beginIndex || endIndex >(jint)data_.size()) throwSIOOBE();
        jint count = 0;
        for (jint i = beginIndex; i < endIndex; ++i) {
            char16_t c = data_[(std::size_t)i];
            if (isHighSurrogate(c) && (i + 1) < endIndex) {
                char16_t c2 = data_[(std::size_t)(i + 1)];
                if (isLowSurrogate(c2)) { ++count; ++i; continue; }
            }
            ++count;
        }
        return count;
    }

    jint String::offsetByCodePoints(jint index, jint codePointOffset) const {
        if (index < 0 || index >(jint)data_.size()) throwSIOOBE();
        jint i = index;

        if (codePointOffset >= 0) {
            jint remaining = codePointOffset;
            while (remaining > 0) {
                if (i >= (jint)data_.size()) throwSIOOBE();
                char16_t c = data_[(std::size_t)i++];
                if (isHighSurrogate(c) && i < (jint)data_.size()) {
                    char16_t c2 = data_[(std::size_t)i];
                    if (isLowSurrogate(c2)) ++i;
                }
                --remaining;
            }
            return i;
        }
        else {
            jint remaining = -codePointOffset;
            while (remaining > 0) {
                if (i <= 0) throwSIOOBE();
                char16_t c2 = data_[(std::size_t)(--i)];
                if (isLowSurrogate(c2) && i > 0) {
                    char16_t c1 = data_[(std::size_t)(i - 1)];
                    if (isHighSurrogate(c1)) --i;
                }
                --remaining;
            }
            return i;
        }
    }

    // =========================
    // Comparison
    // =========================
    jint String::compareTo(jxx::Ptr<String> anotherString) const {
        if (!anotherString) throwNPE();
        const auto& b = anotherString->data_;
        const std::size_t n = std::min(data_.size(), b.size());
        for (std::size_t i = 0; i < n; ++i) {
            if (data_[i] != b[i]) return (jint)data_[i] - (jint)b[i];
        }
        return (jint)data_.size() - (jint)b.size();
    }

    // Java compareToIgnoreCase is Unicode-aware; this is a best-effort ASCII fold.
    // (Full parity requires ICU.)
    jint String::compareToIgnoreCase(jxx::Ptr<String> str) const {
        if (!str) throwNPE();
        auto fold = [](char16_t c) -> char16_t {
            if (c >= u'A' && c <= u'Z') return (char16_t)(c - u'A' + u'a');
            return c;
            };
        const auto& b = str->data_;
        const std::size_t n = std::min(data_.size(), b.size());
        for (std::size_t i = 0; i < n; ++i) {
            char16_t a1 = fold(data_[i]);
            char16_t b1 = fold(b[i]);
            if (a1 != b1) return (jint)a1 - (jint)b1;
        }
        return (jint)data_.size() - (jint)b.size();
    }

    jbool String::equals(jxx::Ptr<Object> obj) const {
        auto s = std::dynamic_pointer_cast<String>(obj);
        if (!s) return false;
        return data_ == s->data_;
    }

    jbool String::equalsIgnoreCase(jxx::Ptr<String> str) const {
        if (!str) return false;
        return compareToIgnoreCase(str) == 0;
    }

    // =========================
    // contains / indexOf / lastIndexOf
    // =========================
    jbool String::contains(jxx::Ptr<CharSequence> s) const {
        if (!s) throwNPE();
        std::u16string needle;
        needle.reserve((std::size_t)s->length());
        for (jint i = 0; i < s->length(); ++i) needle.push_back((char16_t)s->charAt(i));
        if (needle.empty()) return true;
        return data_.find(needle) != std::u16string::npos;
    }

    jint String::indexOf(jint ch) const { return indexOf(ch, 0); }

    jint String::indexOf(jint ch, jint fromIndex) const {
        if (fromIndex < 0) fromIndex = 0;
        if (fromIndex > (jint)data_.size()) return -1;

        // Java: if ch is supplementary, search for surrogate pair
        if (ch > 0xFFFF) {
            jint cp = ch;
            cp -= 0x10000;
            char16_t high = (char16_t)(0xD800 + ((cp >> 10) & 0x3FF));
            char16_t low = (char16_t)(0xDC00 + (cp & 0x3FF));

            for (jint i = fromIndex; i + 1 < (jint)data_.size(); ++i) {
                if (data_[(std::size_t)i] == high && data_[(std::size_t)(i + 1)] == low) return i;
            }
            return -1;
        }

        char16_t c = (char16_t)ch;
        auto pos = data_.find(c, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    jint String::indexOf(jxx::Ptr<String> str) const { return indexOf(str, 0); }

    jint String::indexOf(jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE();
        if (fromIndex < 0) fromIndex = 0;
        if (fromIndex > (jint)data_.size()) return -1;

        const auto& needle = str->data_;
        if (needle.empty()) return fromIndex;

        auto pos = data_.find(needle, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    jint String::lastIndexOf(jint ch) const {
        return lastIndexOf(ch, (jint)data_.size() - 1);
    }

    jint String::lastIndexOf(jint ch, jint fromIndex) const {
        if (data_.empty()) return -1;
        if (fromIndex >= (jint)data_.size()) fromIndex = (jint)data_.size() - 1;
        if (fromIndex < 0) return -1;

        if (ch > 0xFFFF) {
            jint cp = ch;
            cp -= 0x10000;
            char16_t high = (char16_t)(0xD800 + ((cp >> 10) & 0x3FF));
            char16_t low = (char16_t)(0xDC00 + (cp & 0x3FF));

            for (jint i = fromIndex; i - 1 >= 0; --i) {
                if (i + 1 < (jint)data_.size() &&
                    data_[(std::size_t)i] == high &&
                    data_[(std::size_t)(i + 1)] == low) return i;
            }
            return -1;
        }

        char16_t c = (char16_t)ch;
        auto pos = data_.rfind(c, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    jint String::lastIndexOf(jxx::Ptr<String> str) const {
        return lastIndexOf(str, (jint)data_.size());
    }

    jint String::lastIndexOf(jxx::Ptr<String> str, jint fromIndex) const {
        if (!str) throwNPE();
        const auto& needle = str->data_;
        if (needle.empty()) return std::min(fromIndex, (jint)data_.size());

        if (fromIndex > (jint)data_.size()) fromIndex = (jint)data_.size();
        if (fromIndex < 0) return -1;

        auto pos = data_.rfind(needle, (std::size_t)fromIndex);
        return (pos == std::u16string::npos) ? -1 : (jint)pos;
    }

    // =========================
    // startsWith / endsWith
    // =========================
    jbool String::startsWith(jxx::Ptr<String> prefix) const { return startsWith(prefix, 0); }

    jbool String::startsWith(jxx::Ptr<String> prefix, jint toffset) const {
        if (!prefix) throwNPE();
        if (toffset < 0 || toffset >(jint)data_.size()) return false;

        const auto& p = prefix->data_;
        if (p.size() > data_.size() - (std::size_t)toffset) return false;

        return std::equal(p.begin(), p.end(), data_.begin() + (std::size_t)toffset);
    }

    jbool String::endsWith(jxx::Ptr<String> suffix) const {
        if (!suffix) throwNPE();
        const auto& s = suffix->data_;
        if (s.size() > data_.size()) return false;
        return std::equal(s.begin(), s.end(), data_.end() - (std::ptrdiff_t)s.size());
    }

    // =========================
    // substring
    // =========================
    jxx::Ptr<String> String::substring(jint beginIndex) const {
        return substring(beginIndex, (jint)data_.size());
    }

    jxx::Ptr<String> String::substring(jint beginIndex, jint endIndex) const {
        if (beginIndex < 0 || endIndex < beginIndex || endIndex >(jint)data_.size()) throwSIOOBE();
        std::u16string sub = data_.substr((std::size_t)beginIndex, (std::size_t)(endIndex - beginIndex));
        return std::make_shared<String>(utf16_to_utf8(sub).c_str()); // uses utf8 ctor
    }

    // =========================
    // concat
    // =========================
    jxx::Ptr<String> String::concat(jxx::Ptr<String> str) const {
        if (!str) throwNPE();
        if (str->data_.empty()) return std::const_pointer_cast<String>(Object::shared_from_this());
        std::u16string out = data_;
        out += str->data_;
        return std::make_shared<String>(utf16_to_utf8(out).c_str());
    }

    // =========================
    // replace(oldChar,newChar)
    // =========================
    jxx::Ptr<String> String::replace(jchar oldChar, jchar newChar) const {
        std::u16string out = data_;
        for (auto& c : out) {
            if ((jchar)c == oldChar) c = (char16_t)newChar;
        }
        return std::make_shared<String>(utf16_to_utf8(out).c_str());
    }

    // =========================
    // trim (Java 8: remove <= U+0020)
    // =========================
    jxx::Ptr<String> String::trim() const {
        std::size_t start = 0;
        std::size_t end = data_.size();

        while (start < end && data_[start] <= 0x0020) ++start;
        while (end > start && data_[end - 1] <= 0x0020) --end;

        if (start == 0 && end == data_.size()) return std::const_pointer_cast<String>(Object::shared_from_this());

        std::u16string out = data_.substr(start, end - start);
        return std::make_shared<String>(utf16_to_utf8(out).c_str());
    }

    // =========================
    // toLowerCase / toUpperCase (best-effort ASCII)
    // =========================
    jxx::Ptr<String> String::toLowerCase() const { return toLowerCase(nullptr); }

    jxx::Ptr<String> String::toLowerCase(jxx::Ptr<Locale> /*locale*/) const {
        std::u16string out = data_;
        for (auto& c : out) {
            if (c >= u'A' && c <= u'Z') c = (char16_t)(c - u'A' + u'a');
        }
        return std::make_shared<String>(utf16_to_utf8(out).c_str());
    }

    jxx::Ptr<String> String::toUpperCase() const { return toUpperCase(nullptr); }

    jxx::Ptr<String> String::toUpperCase(jxx::Ptr<Locale> /*locale*/) const {
        std::u16string out = data_;
        for (auto& c : out) {
            if (c >= u'a' && c <= u'z') c = (char16_t)(c - u'a' + u'A');
        }
        return std::make_shared<String>(utf16_to_utf8(out).c_str());
    }

    // =========================
    // toCharArray / getChars
    // =========================
    jxx::Ptr<CharArray> String::toCharArray() const {
        auto arr = std::make_shared<CharArray>((std::uint32_t)data_.size());
        for (std::size_t i = 0; i < data_.size(); ++i) {
            (*arr)[(jint)i] = (jchar)data_[i];
        }
        return arr;
    }

    void String::getChars(jint srcBegin, jint srcEnd, jxx::Ptr<CharArray> dst, jint dstBegin) const {
        if (!dst) throwNPE();
        if (srcBegin < 0 || srcEnd < srcBegin || srcEnd >(jint)data_.size()) throwSIOOBE();
        if (dstBegin < 0) throwSIOOBE();
        if ((std::uint32_t)(dstBegin + (srcEnd - srcBegin)) > dst->length) throwSIOOBE();

        for (jint i = 0; i < (srcEnd - srcBegin); ++i) {
            (*dst)[dstBegin + i] = (jchar)data_[(std::size_t)(srcBegin + i)];
        }
    }

    // =========================
    // getBytes
    // =========================
    jxx::Ptr<ByteArray> String::getBytes() const {
        // Deterministic default: UTF-8 (Java uses platform default)
        std::string b = utf16_to_utf8(data_);
        auto arr = std::make_shared<ByteArray>((std::uint32_t)b.size());
        for (std::size_t i = 0; i < b.size(); ++i) {
            (*arr)[(jint)i] = (jbyte)(unsigned char)b[i];
        }
        return arr;
    }

    jxx::Ptr<ByteArray> String::getBytes(jxx::Ptr<Charset> charset) const {
        if (!charset) throwNPE();
        // Charset does the encoding
        return charset->encode(std::const_pointer_cast<String>(Object::shared_from_this()));
    }

    jxx::Ptr<ByteArray> String::getBytes(jxx::Ptr<String> charsetName) const {
        if (!charsetName) throwNPE();
        auto cs = Charset::forName(charsetName);
        return cs->encode(std::const_pointer_cast<String>(Object::shared_from_this()));
    }

    // =========================
    // Regex (best-effort on UTF-8 using std::regex)
    // =========================
    jbool String::matches(jxx::Ptr<String> regex) const {
        if (!regex) throwNPE();
        std::regex re(regex->utf8());
        return std::regex_match(this->utf8(), re);
    }

    jxx::Ptr<String> String::replaceAll(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const {
        if (!regex || !replacement) throwNPE();
        std::regex re(regex->utf8());
        std::string out = std::regex_replace(this->utf8(), re, replacement->utf8());
        return std::make_shared<String>(out.c_str());
    }

    jxx::Ptr<String> String::replaceFirst(jxx::Ptr<String> regex, jxx::Ptr<String> replacement) const {
        if (!regex || !replacement) throwNPE();
        std::regex re(regex->utf8());
        std::string in = this->utf8();
        std::smatch m;
        if (!std::regex_search(in, m, re)) return std::const_pointer_cast<String>(Object::shared_from_this());
        std::string out = in.substr(0, (std::size_t)m.position()) +
            replacement->utf8() +
            in.substr((std::size_t)(m.position() + m.length()));
        return std::make_shared<String>(out.c_str());
    }

    jxx::Ptr<StringArray> String::split(jxx::Ptr<String> regex) const {
        return split(regex, 0);
    }

    // Java split semantics (approx):
    // limit > 0: at most limit parts, last is remainder
    // limit == 0: trailing empty strings discarded
    // limit < 0: all parts including trailing empties
    jxx::Ptr<StringArray> String::split(jxx::Ptr<String> regex, jint limit) const {
        if (!regex) throwNPE();

        std::regex re(regex->utf8());
        std::string s = this->utf8();

        std::vector<std::string> parts;
        parts.reserve(8);

        std::sregex_iterator it(s.begin(), s.end(), re);
        std::sregex_iterator end;

        std::size_t lastPos = 0;
        for (; it != end; ++it) {
            std::size_t matchPos = (std::size_t)it->position();
            std::size_t matchLen = (std::size_t)it->length();

            if (limit > 0 && (jint)parts.size() == (limit - 1)) {
                // remainder
                parts.push_back(s.substr(lastPos));
                lastPos = s.size();
                break;
            }

            parts.push_back(s.substr(lastPos, matchPos - lastPos));
            lastPos = matchPos + matchLen;
        }

        if (!(limit > 0 && (jint)parts.size() == limit)) {
            if (lastPos <= s.size()) parts.push_back(s.substr(lastPos));
        }

        if (limit == 0) {
            // remove trailing empty strings
            while (!parts.empty() && parts.back().empty()) parts.pop_back();
        }

        auto arr = std::make_shared<StringArray>((std::uint32_t)parts.size());
        for (std::size_t i = 0; i < parts.size(); ++i) {
            (*arr)[(jint)i] = std::make_shared<String>(parts[i].c_str());
        }
        return arr;
    }

    // =========================
    // Misc
    // =========================
    jbool String::isEmpty() const { return data_.empty(); }

    jint String::hashCode() const {
        if (hashComputed_) return hash_;
        jint h = 0;
        for (char16_t c : data_) {
            h = 31 * h + (jint)c;
        }
        hash_ = h;
        hashComputed_ = true;
        return hash_;
    }

    jxx::Ptr<String> String::intern() {
        std::lock_guard<std::mutex> lock(internMutex());
        auto& pool = internPool();

        auto it = pool.find(data_);
        if (it != pool.end()) {
            if (auto existing = it->second.lock()) {
                return existing;
            }
        }
        auto self = std::const_pointer_cast<String>(Object::shared_from_this());
        pool[data_] = self;
        return self;
    }

    std::string String::utf8() const {
        return utf16_to_utf8(data_);
    }

    // =========================
    // Static factories
    // =========================
    jxx::Ptr<String> String::valueOf(jbool b) {
        return std::make_shared<String>(b ? "true" : "false");
    }

    jxx::Ptr<String> String::valueOf(jchar c) {
        std::u16string u;
        u.push_back((char16_t)c);
        return std::make_shared<String>(utf16_to_utf8(u).c_str());
    }

    jxx::Ptr<String> String::valueOf(jint i) {
        return std::make_shared<String>(std::to_string(i).c_str());
    }

    jxx::Ptr<String> String::valueOf(jlong l) {
        return std::make_shared<String>(std::to_string((long long)l).c_str());
    }

    jxx::Ptr<String> String::valueOf(jfloat f) {
        std::ostringstream oss;
        oss << f;
        return std::make_shared<String>(oss.str().c_str());
    }

    jxx::Ptr<String> String::valueOf(jdouble d) {
        std::ostringstream oss;
        oss << d;
        return std::make_shared<String>(oss.str().c_str());
    }

    jxx::Ptr<String> String::valueOf(jxx::Ptr<Object> obj) {
        if (!obj) return std::make_shared<String>("null");
        return obj->toString();
    }

    jxx::Ptr<String> String::valueOf(jxx::Ptr<CharArray> data) {
        return copyValueOf(data);
    }

    jxx::Ptr<String> String::valueOf(jxx::Ptr<CharArray> data, jint offset, jint count) {
        return copyValueOf(data, offset, count);
    }

    jxx::Ptr<String> String::copyValueOf(jxx::Ptr<CharArray> data) {
        if (!data) throwNPE();
        auto s = std::make_shared<String>(data);
        return s;
    }

    jxx::Ptr<String> String::copyValueOf(jxx::Ptr<CharArray> data, jint offset, jint count) {
        if (!data) throwNPE();
        auto s = std::make_shared<String>(data, offset, count);
        return s;
    }

} // namespace jxx::lang