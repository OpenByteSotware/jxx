#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <stdexcept>
#include <limits>
#include <regex>
#include <ostream>
#include <cstdint>
#include <algorithm>
#include <utility>
#include <type_traits>
#include <codecvt>
#include <locale>
#include "jxx.lang.internal.h"
#include "jxx.lang.Object.h"
#include "jxx.lang.Comparable.h"

using namespace std;

namespace jxx::lang {

    class String : public Object, Comparable<String> {
    public:
        // ---- Construction (immutable) ----
        String() = default;

        ~String() = default;

        // From UTF-16 (best 1:1 match with Java)
        explicit String(const std::u16string& s) : data_(s) {}
        explicit String(std::u16string&& s) : data_(std::move(s)) {}
        String(const char* s) { assign_from_cstr(s); }
        //explicit String(const char* s, const Allocator& a = Allocator());

        String(std::string_view sv) {
            assign_from_span(sv.data(), sv.size());
        }

        // From UTF-16 view (C++17 has basic_string_view)
        explicit String(std::u16string_view sv) : data_(sv) {}

        // Convert UTF-8 const char* to UTF-16 std::u16string
        static std::u16string utf8_to_utf16(const char* utf8_str) {
            if (!utf8_str) {
                return u""; // Handle null pointer safely
            }

            // Create a converter (UTF-8 <-> UTF-16)
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> convert;

            try {
                return convert.from_bytes(utf8_str);
            }
            catch (const std::range_error& e) {
                std::cerr << "UTF-8 to UTF-16 conversion error: " << e.what() << "\n";
                return u"";
            }
        }

        // From UTF-8 (convenience)
        static String fromUtf8(const std::string& utf8) {
            return String(utf8ToUtf16(utf8));
        }

        // ---- Encoding interop ----
        std::string toUtf8() const {
            return String::utf16ToUtf8(data_);
        }

        std::string toStdString() const {
            return std::string((const char*)this->data_.c_str());
        }

        // Semantically similar to String.format(Locale, String, Object...)
        static String formatLocale(const std::locale& loc, const std::string& format, const std::vector<std::string>& args) {
            // Simple implementation using printf-style for C++17
            // For true Java locale handling, this would require complex ICU usage
            int size_s = std::snprintf(nullptr, 0, format.c_str(), args[0].c_str()) + 1; // Simplified example
            auto size = static_cast<size_t>(size_s);
            auto buf = std::make_unique<char[]>(size);
            std::snprintf(buf.get(), size, format.c_str(), args[0].c_str());
            return String(std::string(buf.get(), buf.get() + size - 1));
        }

        // ---- Core Java-like API ----
        // length(): number of UTF-16 code units (like Java String.length())
        int length() const {
            if (data_.size() > static_cast<size_t>(std::numeric_limits<int>::max())) {
                throw std::overflow_error("length exceeds Java int");
            }
            return static_cast<int>(data_.size());
        }

        bool isEmpty() const { return data_.empty(); }

        char16_t charAt(int index) const {
            checkIndexInclusive(index, 0, length() - 1, "charAt index out of range");
            return data_[static_cast<size_t>(index)];
        }

        // substring(begin)
        String substring(int beginIndex) const {
            return substring(beginIndex, length());
        }

        // substring(begin, end) with Java semantics
        String substring(int beginIndex, int endIndex) const {
            if (beginIndex < 0 || endIndex < beginIndex || endIndex > length()) {
                throw std::out_of_range("substring indices out of range");
            }
            return String(std::u16string(data_.begin() + beginIndex, data_.begin() + endIndex));
        }

        bool startsWith(const String& prefix) const { return startsWith(prefix, 0); }

        bool startsWith(const String& prefix, int toffset) const {
            if (toffset < 0) return false;
            if (toffset + prefix.length() > length()) return false;
            return std::equal(prefix.data_.begin(), prefix.data_.end(), data_.begin() + toffset);
        }

        bool endsWith(const String& suffix) const {
            if (suffix.length() > length()) return false;
            return std::equal(suffix.data_.rbegin(), suffix.data_.rend(), data_.rbegin());
        }

        // indexOf (string)
        int indexOf(const String& str) const { return indexOf(str, 0); }

        int indexOf(const String& str, int fromIndex) const {
            if (fromIndex < 0) fromIndex = 0;
            if (fromIndex > length()) return -1;
            auto pos = data_.find(str.data_, static_cast<size_t>(fromIndex));
            return (pos == std::u16string::npos) ? -1 : static_cast<int>(pos);
        }

        // indexOf (char)
        int indexOf(char16_t ch) const { return indexOf(ch, 0); }

        int indexOf(char16_t ch, int fromIndex) const {
            if (fromIndex < 0) fromIndex = 0;
            if (fromIndex > length()) return -1;
            auto pos = data_.find(ch, static_cast<size_t>(fromIndex));
            return (pos == std::u16string::npos) ? -1 : static_cast<int>(pos);
        }

        // lastIndexOf (string)
        int lastIndexOf(const String& str) const { return lastIndexOf(str, length()); }

        int lastIndexOf(const String& str, int fromIndex) const {
            int right = std::min(fromIndex, length());
            if (str.length() == 0) return right;
            if (str.length() > length()) return -1;
            // emulate Java's lastIndexOf range
            auto pos = data_.rfind(str.data_, static_cast<size_t>(right - str.length()));
            return (pos == std::u16string::npos) ? -1 : static_cast<int>(pos);
        }

        // lastIndexOf (char)
        int lastIndexOf(char16_t ch) const { return lastIndexOf(ch, length() - 1); }

        int lastIndexOf(char16_t ch, int fromIndex) const {
            if (length() == 0) return -1;
            if (fromIndex < 0) return -1;
            int right = std::min(fromIndex, length() - 1);
            for (int i = right; i >= 0; --i) {
                if (data_[static_cast<size_t>(i)] == ch) return i;
            }
            return -1;
        }

        // equals / equalsIgnoreCase
        bool equals(const String& other) const { return data_ == other.data_; }

        bool equalsIgnoreCase(const String& other) const {
            // ASCII-only case fold (for full Unicode, use ICU)
            if (length() != other.length()) return false;
            for (int i = 0; i < length(); ++i) {
                char16_t a = data_[static_cast<size_t>(i)];
                char16_t b = other.data_[static_cast<size_t>(i)];
                if (asciiLower(a) != asciiLower(b)) return false;
            }
            return true;
        }

        // compareTo / compareToIgnoreCase (lexicographic by UTF-16 code unit)
        int compareTo(const String& other) const {
            const auto& a = data_;
            const auto& b = other.data_;
            const size_t n = std::min(a.size(), b.size());
            for (size_t i = 0; i < n; ++i) {
                if (a[i] < b[i]) return -1;
                if (a[i] > b[i]) return 1;
            }
            if (a.size() == b.size()) return 0;
            return (a.size() < b.size()) ? -1 : 1;
        }

        int compareToIgnoreCase(const String& other) const {
            // ASCII-only folding
            const size_t n = std::min(data_.size(), other.data_.size());
            for (size_t i = 0; i < n; ++i) {
                auto ac = asciiLower(data_[i]);
                auto bc = asciiLower(other.data_[i]);
                if (ac < bc) return -1;
                if (ac > bc) return 1;
            }
            if (data_.size() == other.data_.size()) return 0;
            return (data_.size() < other.data_.size()) ? -1 : 1;
        }

        String toLowerCase() const {
            std::u16string out = data_;
            for (auto& c : out) c = asciiLower(c);
            return String(std::move(out));
        }

        String toUpperCase() const {
            std::u16string out = data_;
            for (auto& c : out) c = asciiUpper(c);
            return String(std::move(out));
        }

        // trim(): Java's original trim (<= U+0020)
        String trim() const {
            if (data_.empty()) return *this;
            size_t start = 0;
            size_t end = data_.size();
            while (start < end && data_[start] <= u'\x20') ++start;
            while (end > start && data_[end - 1] <= u'\x20') --end;
            if (start == 0 && end == data_.size()) return *this;
            return String(std::u16string(data_.begin() + start, data_.begin() + end));
        }

        // replace(char, char)
        String replace(char16_t oldChar, char16_t newChar) const {
            if (oldChar == newChar) return *this;
            std::u16string out = data_;
            for (auto& c : out) if (c == oldChar) c = newChar;
            return String(std::move(out));
        }

        // replace(target, replacement) - literal (not regex)
        String replace(const String& target, const String& replacement) const {
            if (target.isEmpty()) return *this;
            std::u16string out;
            out.reserve(data_.size());
            size_t i = 0;
            while (i < data_.size()) {
                if (i + target.data_.size() <= data_.size() &&
                    std::equal(target.data_.begin(), target.data_.end(), data_.begin() + i)) {
                    out.append(replacement.data_);
                    i += target.data_.size();
                }
                else {
                    out.push_back(data_[i++]);
                }
            }
            return String(std::move(out));
        }

        // split(regex, limit) — implemented via UTF-8 + std::regex (ECMAScript), not Java regex.
        // Semantics: tries to mimic Java's split:
        //   limit == 0  -> discard trailing empty strings
        //   limit > 0   -> at most 'limit' pieces, last may contain separators
        //   limit < 0   -> unlimited; keep trailing empty
        std::vector<String> split(const String& regex, int limit = 0) const {
            // Convert to UTF-8 for std::regex (ECMAScript).
            // NOTE: Java regex != std::regex (ECMAScript). For Java-compatible regex, use a proper library.
            const std::string s8 = String::utf16_to_utf8(data_.c_str());
            const std::string p8 = regex.toUtf8().c_str();
            std::regex re(p8, std::regex::ECMAScript);

            std::vector<String> parts;
            if (limit == 1) { // Java: return original string
                parts.push_back(*this);
                return parts;
            }

            std::sregex_token_iterator it(s8.begin(), s8.end(), re, -1);
            std::sregex_token_iterator end;

            for (; it != end; ++it) {
                parts.emplace_back(String::fromUtf8(it->str()));
                if (limit > 0 && static_cast<int>(parts.size()) == limit - 1) {
                    // Append remainder of the string (including any separators)
                    std::smatch m;
                    std::string::const_iterator search_start = it->second;
                    if (std::regex_search(search_start, s8.cend(), m, re)) {
                        // The rest after the last split point up to end of input,
                        // including unmatched remainder (emulating Java behavior).
                    }
                    // Compute the remaining substring directly:
                    auto consumed = it->second - s8.begin();
                    parts.emplace_back(String::fromUtf8(std::string(s8.begin() + (it->first - s8.begin()) + it->length(), s8.end())));
                    return parts;
                }
            }

            // Add last token if iterator produced it (already included by token iterator).
            // Apply Java's trailing-empty removal if limit == 0
            if (limit == 0) {
                while (!parts.empty() && parts.back().isEmpty()) {
                    parts.pop_back();
                }
            }

            return parts;
        }

        // concat and operators
        String concat(const String& other) const {
            if (other.isEmpty()) return *this;
            if (isEmpty()) return other;
            std::u16string out;
            out.reserve(data_.size() + other.data_.size());
            out.append(data_);
            out.append(other.data_);
            return String(std::move(out));
        }

        friend String operator+(const String& a, const String& b) {
            return a.concat(b);
        }

        // equals/!= operators map to equals()
        friend bool operator==(const String& a, const String& b) { return a.equals(b); }
     
        friend bool operator!=(const String& a, const String& b) { return !a.equals(b); }

        // hashCode(): Java-compatible 32-bit signed
        jint hashCode() const {
            if (!hash_cached_) {
                uint32_t h = 0;
                for (char16_t c : data_) {
                    h = h * 31u + static_cast<uint32_t>(c);
                }
                hash_cache_ = static_cast<int32_t>(h); // wrap like Java
                hash_cached_ = true;
            }
            return hash_cache_;
        }

        // For interop and utilities
        const std::u16string& utf16() const { return data_; }

        // Stream as UTF-8
        friend std::ostream& operator<<(std::ostream& os, const String& s) {
            return os << s.toUtf8();
        }

    private:
        std::u16string data_{};

        // Java caches hashCode
        mutable bool hash_cached_ = false;
        mutable int32_t hash_cache_ = 0;

        static void checkIndexInclusive(int idx, int lo, int hi, const char* msg) {
            if (idx < lo || idx > hi) throw std::out_of_range(msg);
        }

        static char16_t asciiLower(char16_t c) {
            return (c >= u'A' && c <= u'Z') ? static_cast<char16_t>(c + 32) : c;
        }
        static char16_t asciiUpper(char16_t c) {
            return (c >= u'a' && c <= u'z') ? static_cast<char16_t>(c - 32) : c;
        }

        static std::string utf16_to_utf8(const std::u16string& u16) {
            std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> conv;
            return conv.to_bytes(u16);
        }
        std::u16string get_data() { return data_.c_str(); }

        // ---- UTF-8 <-> UTF-16 conversion (no locales; replaces invalid with U+FFFD) ----
        static std::u16string utf8ToUtf16(const std::string& s) {
            std::u16string out;
            out.reserve(s.size()); // rough
            size_t i = 0;
            while (i < s.size()) {
                uint8_t b0 = static_cast<uint8_t>(s[i]);
                uint32_t codepoint = 0;
                size_t needed = 0;

                if (b0 <= 0x7F) { codepoint = b0; needed = 1; }
                else if ((b0 & 0xE0) == 0xC0) { codepoint = b0 & 0x1F; needed = 2; }
                else if ((b0 & 0xF0) == 0xE0) { codepoint = b0 & 0x0F; needed = 3; }
                else if ((b0 & 0xF8) == 0xF0) { codepoint = b0 & 0x07; needed = 4; }
                else { out.push_back(u'\uFFFD'); ++i; continue; }

                if (i + needed > s.size()) { out.push_back(u'\uFFFD'); break; }

                bool invalid = false;
                for (size_t j = 1; j < needed; ++j) {
                    uint8_t bx = static_cast<uint8_t>(s[i + j]);
                    if ((bx & 0xC0) != 0x80) { invalid = true; break; }
                    codepoint = (codepoint << 6) | (bx & 0x3F);
                }

                // Adjust accumulated codepoint for leading byte bits
                if (needed == 2) codepoint &= 0x7FF;
                else if (needed == 3) codepoint &= 0xFFFF;
                else if (needed == 4) codepoint &= 0x1FFFFF;

                // Overlong / range checks (simplified but sufficient)
                if (invalid ||
                    (needed == 2 && codepoint < 0x80) ||
                    (needed == 3 && codepoint < 0x800) ||
                    (needed == 4 && codepoint < 0x10000) ||
                    (codepoint > 0x10FFFF) ||
                    (codepoint >= 0xD800 && codepoint <= 0xDFFF)) {
                    out.push_back(u'\uFFFD');
                    i += std::max<size_t>(1, needed);
                    continue;
                }

                if (codepoint <= 0xFFFF) {
                    out.push_back(static_cast<char16_t>(codepoint));
                }
                else {
                    codepoint -= 0x10000;
                    char16_t high = static_cast<char16_t>(0xD800 + (codepoint >> 10));
                    char16_t low = static_cast<char16_t>(0xDC00 + (codepoint & 0x3FF));
                    out.push_back(high);
                    out.push_back(low);
                }
                i += needed;
            }
            return out;
        }

        static std::string utf16ToUtf8(const std::u16string& s) {
            std::string out;
            out.reserve(s.size()); // rough
            for (size_t i = 0; i < s.size(); ++i) {
                uint32_t codepoint = 0;
                char16_t w1 = s[i];
                if (w1 >= 0xD800 && w1 <= 0xDBFF) {
                    if (i + 1 < s.size()) {
                        char16_t w2 = s[i + 1];
                        if (w2 >= 0xDC00 && w2 <= 0xDFFF) {
                            codepoint = 0x10000 + (((w1 - 0xD800) << 10) | (w2 - 0xDC00));
                            ++i;
                        }
                        else {
                            codepoint = 0xFFFD; // unpaired high surrogate
                        }
                    }
                    else {
                        codepoint = 0xFFFD; // truncated high surrogate
                    }
                }
                else if (w1 >= 0xDC00 && w1 <= 0xDFFF) {
                    codepoint = 0xFFFD; // unpaired low surrogate
                }
                else {
                    codepoint = w1;
                }

                if (codepoint <= 0x7F) {
                    out.push_back(static_cast<char>(codepoint));
                }
                else if (codepoint <= 0x7FF) {
                    out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
                    out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                }
                else if (codepoint <= 0xFFFF) {
                    out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
                    out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                    out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                }
                else {
                    out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
                    out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
                    out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
                    out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
                }
            }
            return out;
        }

        static std::size_t safe_strlen(const char* s) noexcept {
            if (!s) return 0;
            const char* p = s;
            while (*p) ++p;
            return static_cast<std::size_t>(p - s);
        }


        void assign_from_span(const char* p, std::size_t n) {
            char* new_data = new char[n + 1];
            for (std::size_t i = 0; i < n; ++i) new_data[i] = p[i];
            new_data[n] = '\0';
            // now commit
            //delete[] data_;
            data_ = utf8ToUtf16(new_data);
        }

        void assign_from_cstr(const char* s) {
            // Permit nullptr like many custom strings (std::string is UB for nullptr).
            const std::size_t n = safe_strlen(s);
            allocate_and_copy(s, n);
        }

        void allocate_and_copy(const char* s, std::size_t n) {
            char* new_data = new char[n + 1];   // +1 for NUL
            for (std::size_t i = 0; i < n; ++i) {
                new_data[i] = s[i];
            }
            new_data[n] = '\0';

            //delete[] data_;
            data_ = utf8ToUtf16(new_data);
        }
    };
}

namespace std {
    template <>
    struct hash<jxx::lang::String> {
        std::size_t operator()(const jxx::lang::String& s) const noexcept {
            return static_cast<size_t>(static_cast<uint32_t>(s.hashCode()));
        }
    };
}