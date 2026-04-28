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
#include "lang/jxx_types.h"
#include "lang/jxx.lang.Object.h"
#include "lang/jxx.lang.Comparable.h"

using namespace std;

namespace jxx::lang {

    class String : public Object, Comparable<String> {
    public:
        // ---- Construction (immutable) ----
        String() = default;
        ~String() = default;

        // From UTF-16 (best 1:1 match with Java)
        explicit String(const std::u16string& s);
        explicit String(std::u16string&& s);
        String(const char* s);
        //explicit String(const char* s, const Allocator& a = Allocator());

        String(std::string_view sv);

        // From UTF-16 view (C++17 has basic_string_view)
        explicit String(std::u16string_view sv);

        // Convert UTF-8 const char* to UTF-16 std::u16string
        static std::u16string utf8_to_utf16(const char* utf8_str);         
             
        // Copy constructor
        String(const String& other);
        // Copy assignment operator
        String& operator=(const String& other);

        // From UTF-16 (best 1:1 match with Java)
        explicit String(const std::u16string& s);
        explicit String(std::u16string&& s);
        String(const char* s);

        // From UTF-8 (convenience)
        static String fromUtf8(const std::string& utf8);

        // ---- Encoding jinterop ----
        std::string toUtf8() const;

        std::string toStdString() const;

        // Semantically similar to String.format(Locale, String, Object...)
        static String formatLocale(const std::locale& loc, const std::string& format, const std::vector<std::string>& args);

        // ---- Core Java-like API ----
        // length(): number of UTF-16 code units (like Java String.length())
        jint length() const;

        bool isEmpty() const;

        char16_t charAt(jint index) const;
        // substring(begin)
        String substring(jint beginIndex) const;
        // substring(begin, end) with Java semantics
        String substring(jint beginIndex, jint endIndex) const;

        bool startsWith(const String& prefix) const;

        bool startsWith(const String& prefix, jint toffset) const;

        bool endsWith(const String& suffix) const;
        // indexOf (string)
        jint indexOf(const String& str) const;

        jint indexOf(const String& str, jint fromIndex) const;

        // indexOf (char)
        jint indexOf(char16_t ch) const;

        jint indexOf(char16_t ch, jint fromIndex) const;

        // lastIndexOf (string)
        jint lastIndexOf(const String& str) const;

        jint lastIndexOf(const String& str, jint fromIndex) const;
        // lastIndexOf (char)
        jint lastIndexOf(char16_t ch) const;

        jint lastIndexOf(char16_t ch, jint fromIndex) const;
        
        // equals / equalsIgnoreCase
        bool equals(const String& other) const;

        bool equalsIgnoreCase(const String& other) const;

        // compareTo / compareToIgnoreCase (lexicographic by UTF-16 code unit)
        jint compareTo(const String& other) const;

        jint compareToIgnoreCase(const String& other) const;

        String toLowerCase() const;

        String toUpperCase() const;
        // trim(): Java's original trim (<= U+0020)
        String trim() const;

        // replace(char, char)
        String replace(char16_t oldChar, char16_t newChar) const;

        // replace(target, replacement) - literal (not regex)
        String replace(const String& target, const String& replacement) const;

        // split(regex, limit) — implemented via UTF-8 + std::regex (ECMAScript), not Java regex.
        // Semantics: tries to mimic Java's split:
        //   limit == 0  -> discard trailing empty strings
        //   limit > 0   -> at most 'limit' pieces, last may contain separators
        //   limit < 0   -> unlimited; keep trailing empty
        std::vector<String> split(const String& regex, jint limit = 0) const;
          
        // concat and operators
        String concat(const String& other) const;

        friend String operator+(const String& a, const String& b);

        // equals/!= operators map to equals()
        friend bool operator==(const String& a, const String& b);
     
        friend bool operator!=(const String& a, const String& b);

        // hashCode(): Java-compatible 32-bit signed
        jint hashCode() const;

        // For jinterop and utilities
        const std::u16string& utf16() const;

        // Stream as UTF-8
        friend std::ostream& operator<<(std::ostream& os, const String& s);

    private:
        std::u16string data_{};

        // Java caches hashCode
        mutable bool hash_cached_ = false;
        mutable int32_t hash_cache_ = 0;

        static void checkIndexInclusive(jint idx, jint lo, jint hi, const char* msg);

        static char16_t asciiLower(char16_t c);
        static char16_t asciiUpper(char16_t c);

        static std::string utf16_to_utf8(const std::u16string& u16);
        std::u16string get_data();
        // ---- UTF-8 <-> UTF-16 conversion (no locales; replaces invalid with U+FFFD) ----
        static std::u16string utf8ToUtf16(const std::string& s);

        static std::string utf16ToUtf8(const std::u16string& s);
        static std::size_t safe_strlen(const char* s) noexcept;


        void assign_from_span(const char* p, std::size_t n);

        void assign_from_cstr(const char* s);

        void allocate_and_copy(const char* s, std::size_t n);
    };
}

namespace std {
    template <>
    struct hash<jxx::lang::String> {
        std::size_t operator()(const jxx::lang::String& s) const noexcept {
            return static_cast<size_t>(static_cast<int32_t>(s.hashCode()));
        }
    };
}